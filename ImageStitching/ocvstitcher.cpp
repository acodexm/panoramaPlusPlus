#include "stdafx.h"
#include <iostream>
#include "../Comparator/logger.h"
#include <fstream>
#include <filesystem>
#include <stdio.h>
#include <sys/stat.h>
#include <string>
#include "stitch.h"
#include "cropp.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/stitching/detail/autocalib.hpp"
#include "opencv2/stitching/detail/blenders.hpp"
#include "opencv2/stitching/detail/camera.hpp"
#include "opencv2/stitching/detail/exposure_compensate.hpp"
#include "opencv2/stitching/detail/matchers.hpp"
#include "opencv2/stitching/detail/motion_estimators.hpp"
#include "opencv2/stitching/detail/seam_finders.hpp"
#include "opencv2/stitching/detail/util.hpp"
#include "opencv2/stitching/detail/warpers.hpp"
#include "opencv2/stitching/warpers.hpp"

using namespace std;
using namespace cv;
using namespace cv::detail;
namespace fs = std::filesystem;

#define TAG "customized stitcher "
#define ENABLE_LOG true
#define LOGLN(debug, msg) if(debug) { LOG() << TAG << msg << endl ; }



void cropp(Mat& result);

/*************
 * STEPS & PROGRESS VALUES
 ************/
int FINDER_STEP = 15;
int MATCHER_STEP = 10;
int ESTIMATOR_STEP = 5;
int ADJUSTER_STEP = 5;
int WARPER_STEP = 10;
int COMPENSATOR_STEP = 20;
int SEAM_STEP = 10;
int COMPOSITOR_STEP = 30;


/*************
 * PARAMETERS
 ************/
namespace {

	/** ??? **/
	bool preview = false;

	/** don't support cuda at this time ... **/
	bool try_cuda = false;

	/** neither openCL **/
	bool try_ocl = false;

	/** working resolution **/
	double work_megapix = 0.35;

	/** seam finding resolution **/
	double seam_megapix = 0.2;

	/** final panorama resolution **/
	double compose_megapix = 1.2;

	/** Threshold for two images are from the same panorama confidence. **/
	float conf_thresh = 0.7f;	// frequent crashes when < 0.7

	/** Bundle adjustment cost function. reproj don't seems suitable for spherical pano**/
	string ba_cost_func = "ray"; 	//["reproj", "ray" : "ray"]

	/** Set refinement mask for bundle adjustment. It looks like 'x_xxx'
		where 'x' means refine respective parameter and '_' means don't
		refine one, and has the following format:
		<fx><skew><ppx><aspect><ppy>. The default mask is 'xxxxx'. If bundle
		adjustment doesn't support estimation of selected parameter then
		the respective flag is ignored. **/
	string ba_refine_mask = "xxxxx";

	/** if we should keep horizon straight **/
	bool do_wave_correct = true;

	WaveCorrectKind wave_correct = detail::WAVE_CORRECT_HORIZ;
	bool save_graph = false;
	string save_graph_to;

	/** Warp surface type. **/
	string warp_type = "spherical";

	/** Exposure compensation method. **/
	int expos_comp_type = ExposureCompensator::GAIN;

	/** Confidence for feature matching step. **/
	float match_conf = 0.25f;

	/** Seam estimation method. **/
	string seam_find_type = "dp_colorgrad";	//dp_** is WAY faster!!!

	/** Blending method. **/
	int blend_type = Blender::MULTI_BAND;

	/** Blending strength from [0,100] range. **/
	float blend_strength = 5;


	/** orb featureFinder parameters **/
	Size ORB_GRID_SIZE = Size(1, 1);
	size_t ORB_FEATURES_N = 3500;
}
/*************
 * ATTRIBUTES
 ************/

 /** final panorama name **/
static string _resultPath="result.jpg";

/** loaded images loaded **/
vector<String> _imagesPath;
int _nbImages;

/** current progression of the stitching **/
float _progress = -1;

/** mask used to know what images should we match together **/
static UMat _matchingMask;

/** indices of used images **/
vector<int> _indices;

float _progressStep = 1;
int stitch(int argc, char** argv)
{
	_progress = 0;



	if (argc == 1)
	{
		LOGLN(debug, "not enaugh arguments");
		return -1;
	}

	for (int i = 1; i < argc; ++i) {
		if (string(argv[i]) == "--cuda") {
			try_cuda = true;
		}
		else if (string(argv[i]).find("--") != string::npos) {
			continue;
		}
		else
		{
			struct stat info;

			char* pathname = argv[i];
			if (stat(pathname, &info) != 0)
			{
				printf("cannot access %s\n", pathname);
				continue;
			}
			else if (info.st_mode & S_IFDIR)
				printf("%s is a directory\n", pathname);
			else
			{
				printf("%s is no directory\n", pathname);
				continue;
			}
			for (const auto& entry : fs::directory_iterator(pathname))
			{
				cout << entry.path() << endl;
				_imagesPath.push_back(entry.path().u8string());
			}
		}
	}


#if ENABLE_LOG
	LOGLN(debug,  "Compose panorama...");
	int64 app_start_time = getTickCount();
#endif

	cv::setBreakOnError(true);

	// Check if have enough images
	_nbImages = static_cast<int>(_imagesPath.size());
	if (_nbImages < 2)
	{
		LOGLN(debug,  "Not enaugh images...");
		return -1;
	}

	double work_scale = 1, seam_scale = 1, compose_scale = 1;
	bool is_work_scale_set = false, is_seam_scale_set = false, is_compose_scale_set = false;

	// ================ Finding features... ==================
#if ENABLE_LOG
	LOGLN(debug,  "Finding features...");
	int64 t = getTickCount();
#endif

	_progressStep = ((float)FINDER_STEP / (float)_nbImages);
	Ptr<FeaturesFinder> finder = new OrbFeaturesFinder(ORB_GRID_SIZE, ORB_FEATURES_N);
	Mat full_img, img;
	vector<ImageFeatures> features(_nbImages);
	vector<Mat> images(_nbImages);
	vector<Size> full_img_sizes(_nbImages);
	double seam_work_aspect = 1;

	for (int i = 0; i < _nbImages; ++i)
	{
		full_img = imread(_imagesPath[i]);
		full_img_sizes[i] = full_img.size();

		if (full_img.empty())
		{
			LOGLN(debug,  "Can't open image %s" << _imagesPath[i].c_str());
			return -1;
		}
		if (work_megapix < 0)
		{
			img = full_img;
			work_scale = 1;
			is_work_scale_set = true;
		}
		else
		{
			if (!is_work_scale_set)
			{
				work_scale = min(1.0, sqrt(work_megapix * 1e6 / full_img.size().area()));
				is_work_scale_set = true;
			}
			resize(full_img, img, Size(), work_scale, work_scale);
		}
		if (!is_seam_scale_set)
		{
			seam_scale = min(1.0, sqrt(seam_megapix * 1e6 / full_img.size().area()));
			seam_work_aspect = seam_scale / work_scale;
			is_seam_scale_set = true;
		}

		(*finder)(img, features[i]);
		features[i].img_idx = i;
		LOGLN(debug, "Features in image #" << i + 1 << ": " << features[i].keypoints.size());

		resize(full_img, img, Size(), seam_scale, seam_scale);
		images[i] = img.clone();

		_progress += _progressStep;
	}

	finder->collectGarbage();
	full_img.release();
	img.release();

	LOGLN(debug,  "Finding features, time: " << ((getTickCount() - t) / getTickFrequency())<<" sec");

	// ================ Pairwise matching... ==================
#if ENABLE_LOG
	LOGLN(debug,  "Pairwise matching");
	t = getTickCount();
#endif


	vector<MatchesInfo> pairwise_matches;
	BestOf2NearestMatcher matcher(try_cuda, match_conf);
	matcher(features, pairwise_matches, _matchingMask);
	matcher.collectGarbage();
	_progress += MATCHER_STEP;

	LOGLN(debug,  "Pairwise matching, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");



	// Check if we should save matches graph
	if (save_graph)
	{
		LOGLN(debug, "Saving matches graph...");
		ofstream f(save_graph_to.c_str());
		f << matchesGraphAsString(_imagesPath, pairwise_matches, conf_thresh);
	}

	// Leave only images we are sure are from the same panorama
	_indices = leaveBiggestComponent(features, pairwise_matches, conf_thresh);



	vector<Mat> img_subset;
	vector<String> _imagesPath_subset;
	vector<Size> full_img_sizes_subset;
	for (size_t i = 0; i < _indices.size(); ++i)
	{
		_imagesPath_subset.push_back(_imagesPath[_indices[i]]);
		img_subset.push_back(images[_indices[i]]);
		full_img_sizes_subset.push_back(full_img_sizes[_indices[i]]);
	}

	images = img_subset;
	_imagesPath = _imagesPath_subset;
	full_img_sizes = full_img_sizes_subset;

	// Check if we still have enough images
	_nbImages = static_cast<int>(_imagesPath.size());
	if (_nbImages < 2)
	{
		LOGLN(debug,  "Need more images");

		return -1;
	}
	// ================ estimate homography... ==================
	LOGLN(debug,  "estimate homography");

	HomographyBasedEstimator estimator;
	vector<CameraParams> cameras;
	estimator(features, pairwise_matches, cameras);


	for (size_t i = 0; i < cameras.size(); ++i)
	{
		Mat R;
		cameras[i].R.convertTo(R, CV_32F);
		cameras[i].R = R;
		LOGLN(debug,  "Initial intrinsics #" << _indices[i] + 1 << ":\n" << cameras[i].K());
	}
	_progress += ESTIMATOR_STEP;

	// ================ adjuster... ==================
	Ptr<detail::BundleAdjusterBase> adjuster;
	if (ba_cost_func == "reproj") adjuster = new detail::BundleAdjusterReproj();
	else if (ba_cost_func == "ray") adjuster = new detail::BundleAdjusterRay();
	else
	{
		LOGLN(debug, "Unknown bundle adjustment cost function: '" << ba_cost_func << "'.\n");
		return -1;
	}
	adjuster->setConfThresh(conf_thresh);
	Mat_<uchar> refine_mask = Mat::zeros(3, 3, CV_8U);
	if (ba_refine_mask[0] == 'x') refine_mask(0, 0) = 1;
	if (ba_refine_mask[1] == 'x') refine_mask(0, 1) = 1;
	if (ba_refine_mask[2] == 'x') refine_mask(0, 2) = 1;
	if (ba_refine_mask[3] == 'x') refine_mask(1, 1) = 1;
	if (ba_refine_mask[4] == 'x') refine_mask(1, 2) = 1;
	adjuster->setRefinementMask(refine_mask);
	LOGLN(debug,  "adjusting bundle..");

	(*adjuster)(features, pairwise_matches, cameras);
	_progress += ADJUSTER_STEP;


	// Find median focal length
	LOGLN(debug,  "Find median focal length");

	vector<double> focals;
	for (size_t i = 0; i < cameras.size(); ++i)
	{
		LOGLN(debug,  "Camera #" << _indices[i] + 1 << ":\n" << cameras[i].K());

		focals.push_back(cameras[i].focal);
	}

	sort(focals.begin(), focals.end());
	float warped_image_scale;
	if (focals.size() % 2 == 1)
		warped_image_scale = static_cast<float>(focals[focals.size() / 2]);
	else
		warped_image_scale = static_cast<float>(focals[focals.size() / 2 - 1] + focals[focals.size() / 2]) * 0.5f;

	if (do_wave_correct)
	{
		vector<Mat> rmats;
		for (size_t i = 0; i < cameras.size(); ++i)
			rmats.push_back(cameras[i].R);
		waveCorrect(rmats, wave_correct);
		for (size_t i = 0; i < cameras.size(); ++i)
			cameras[i].R = rmats[i];
	}
	// ================ Warping images... ==================

#if ENABLE_LOG
	LOGLN(debug,  "Warping images (auxiliary)... ");

	t = getTickCount();
#endif

	vector<Point> corners(_nbImages);
	vector<UMat> masks_warped(_nbImages);
	vector<Mat> images_warped(_nbImages);
	vector<Size> sizes(_nbImages);
	vector<Mat> masks(_nbImages);

	// Preapre images masks
	for (int i = 0; i < _nbImages; ++i)
	{
		masks[i].create(images[i].size(), CV_8U);
		masks[i].setTo(Scalar::all(255));
	}

	// Warp images and their masks

	Ptr<WarperCreator> warper_creator;



	if (warp_type == "plane")
		warper_creator = new cv::PlaneWarper();
	else if (warp_type == "cylindrical")
		warper_creator = new cv::CylindricalWarper();
	else if (warp_type == "spherical")
		warper_creator = new cv::SphericalWarper();
	else if (warp_type == "fisheye")
		warper_creator = new cv::FisheyeWarper();
	else if (warp_type == "stereographic")
		warper_creator = new cv::StereographicWarper();


	if (!warper_creator)
	{
		LOGLN(debug, "Can't create the following warper '" << warp_type << "'\n");
		return 1;
	}

	Ptr<RotationWarper> warper = warper_creator->create(static_cast<float>(warped_image_scale * seam_work_aspect));
	_progressStep = (float)WARPER_STEP / (float)_nbImages;
	for (int i = 0; i < _nbImages; ++i)
	{
		Mat_<float> K;
		cameras[i].K().convertTo(K, CV_32F);
		float swa = (float)seam_work_aspect;
		K(0, 0) *= swa; K(0, 2) *= swa;
		K(1, 1) *= swa; K(1, 2) *= swa;

		corners[i] = warper->warp(images[i], K, cameras[i].R, INTER_LINEAR, BORDER_REFLECT, images_warped[i]);
		sizes[i] = images_warped[i].size();

		warper->warp(masks[i], K, cameras[i].R, INTER_NEAREST, BORDER_CONSTANT, masks_warped[i]);
		_progress += _progressStep;
	}

	vector<UMat> images_warped_f(_nbImages);
	for (int i = 0; i < _nbImages; ++i)
		images_warped[i].convertTo(images_warped_f[i], CV_32F);

	images_warped.clear();

	LOGLN(debug,  "Warping images, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");

	// ================ Compensate exposure... ==================
	LOGLN(debug,  "Compensate exposure");
	Ptr<ExposureCompensator> compensator = ExposureCompensator::createDefault(expos_comp_type);
	//TODO : to remove
	//compensator->feed(corners, images_warped, masks_warped);
	compensator->feed(corners, images_warped_f, masks_warped);
	_progress += COMPENSATOR_STEP;

	Ptr<SeamFinder> seam_finder;
	if (seam_find_type == "no")
		seam_finder = new detail::NoSeamFinder();
	else if (seam_find_type == "voronoi")
		seam_finder = new detail::VoronoiSeamFinder();
	else if (seam_find_type == "gc_color")
	{
#ifdef HAVE_OPENCV_CUDA
		if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
			seam_finder = new detail::GraphCutSeamFinderGpu(GraphCutSeamFinderBase::COST_COLOR);
		else
#endif
			seam_finder = new detail::GraphCutSeamFinder(GraphCutSeamFinderBase::COST_COLOR);

	}
	else if (seam_find_type == "gc_colorgrad")
	{
#ifdef HAVE_OPENCV_CUDA
		if (try_cuda && cuda::getCudaEnabledDeviceCount() > 0)
			seam_finder = new detail::GraphCutSeamFinderGpu(GraphCutSeamFinderBase::COST_COLOR_GRAD);
		else
#endif
			seam_finder = new detail::GraphCutSeamFinder(GraphCutSeamFinderBase::COST_COLOR_GRAD);
	}
	else if (seam_find_type == "dp_color")
		seam_finder = new detail::DpSeamFinder(DpSeamFinder::COLOR);
	else if (seam_find_type == "dp_colorgrad")
		seam_finder = new detail::DpSeamFinder(DpSeamFinder::COLOR_GRAD);
	if (!seam_finder)
	{
		LOGLN(debug, "Can't create the following seam finder '" << seam_find_type << "'\n");
		return 1;
	}
	// ================ finding seam... ==================

	LOGLN(debug,  "finding seam");

	seam_finder->find(images_warped_f, corners, masks_warped);
	_progress += SEAM_STEP;

	// Release unused memory
	images.clear();
	images_warped_f.clear();
	masks.clear();


	// ================ Compositing... ==================
#if ENABLE_LOG
	LOGLN(debug,  "Compositing...");
	t = getTickCount();
#endif

	Mat img_warped, img_warped_s;
	Mat dilated_mask, seam_mask, mask, mask_warped;
	Ptr<Blender> blender;
	//double compose_seam_aspect = 1;
	double compose_work_aspect = 1;
	_progressStep = (float)COMPOSITOR_STEP / (float)_nbImages;
	for (int img_idx = 0; img_idx < _nbImages; ++img_idx)
	{
		LOGLN(debug,  "Compositing image #" << _indices[img_idx] + 1);

		// Read image and resize it if necessary
		full_img = imread(_imagesPath[img_idx]);
		if (!is_compose_scale_set)
		{
			if (compose_megapix > 0)
				compose_scale = min(1.0, sqrt(compose_megapix * 1e6 / full_img.size().area()));
			is_compose_scale_set = true;

			// Compute relative scales
			//compose_seam_aspect = compose_scale / seam_scale;
			compose_work_aspect = compose_scale / work_scale;

			// Update warped image scale
			warped_image_scale *= static_cast<float>(compose_work_aspect);
			warper = warper_creator->create(warped_image_scale);

			// Update corners and sizes
			for (int i = 0; i < _nbImages; ++i)
			{
				// Update intrinsics
				cameras[i].focal *= compose_work_aspect;
				cameras[i].ppx *= compose_work_aspect;
				cameras[i].ppy *= compose_work_aspect;

				// Update corner and size
				Size sz = full_img_sizes[i];
				if (abs(compose_scale - 1) > 1e-1)
				{
					sz.width = cvRound(full_img_sizes[i].width * compose_scale);
					sz.height = cvRound(full_img_sizes[i].height * compose_scale);
				}

				Mat K;
				cameras[i].K().convertTo(K, CV_32F);
				Rect roi = warper->warpRoi(sz, K, cameras[i].R);
				corners[i] = roi.tl();
				sizes[i] = roi.size();
			}
		}
		if (abs(compose_scale - 1) > 1e-1)
			resize(full_img, img, Size(), compose_scale, compose_scale);
		else
			img = full_img;
		full_img.release();
		Size img_size = img.size();

		Mat K;
		cameras[img_idx].K().convertTo(K, CV_32F);

		// Warp the current image
		warper->warp(img, K, cameras[img_idx].R, INTER_LINEAR, BORDER_REFLECT, img_warped);
		img.release();

		// Warp the current image mask
		mask.create(img_size, CV_8U);
		mask.setTo(Scalar::all(255));
		warper->warp(mask, K, cameras[img_idx].R, INTER_NEAREST, BORDER_CONSTANT, mask_warped);
		mask.release();

		// Compensate exposure
		compensator->apply(img_idx, corners[img_idx], img_warped, mask_warped);

		img_warped.convertTo(img_warped_s, CV_16S);
		img_warped.release();

		dilate(masks_warped[img_idx], dilated_mask, Mat());
		resize(dilated_mask, seam_mask, mask_warped.size());
		mask_warped = seam_mask & mask_warped;

		if (!blender)
		{
			blender = Blender::createDefault(blend_type, try_cuda);
			Size dst_sz = resultRoi(corners, sizes).size();
			float blend_width = sqrt(static_cast<float>(dst_sz.area())) * blend_strength / 100.f;
			if (blend_width < 1.f)
				blender = Blender::createDefault(Blender::NO, try_cuda);
			else if (blend_type == Blender::MULTI_BAND)
			{
				MultiBandBlender* mb = dynamic_cast<MultiBandBlender*>(static_cast<Blender*>(blender));
				mb->setNumBands(static_cast<int>(ceil(log(blend_width) / log(2.)) - 1.));
				LOGLN(debug,  "Multi-band blender, number of bands: " << mb->numBands());
			}
			else if (blend_type == Blender::FEATHER)
			{
				FeatherBlender* fb = dynamic_cast<FeatherBlender*>(static_cast<Blender*>(blender));
				fb->setSharpness(1.f / blend_width);
				LOGLN(debug,  "Feather blender, sharpness: " << fb->sharpness());
			}
			blender->prepare(corners, sizes);
		}

		// Blend the current image
		blender->feed(img_warped_s, mask_warped, corners[img_idx]);
		_progress += _progressStep;
	}

	Mat result, result_mask;
	blender->blend(result, result_mask);

	LOGLN(debug,  "Compositing, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");
	LOGLN(debug,  "cropping...");
	cropp(result);
	LOGLN(debug,  "saving...");
	imwrite(_resultPath, result);

	LOGLN(debug,  "Finished, total time: " << ((getTickCount() - app_start_time) / getTickFrequency()) << " sec");

	return 0;
}


int getProgress()
{
	return (int)_progress;
}