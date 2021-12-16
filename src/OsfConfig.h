#pragma once
struct OsfConfig
{
	double envelopX = 192.0;
	double envelopY = 108.0;
	double envelopZ = 200.0;
	int resolutionX = 1920;
	int resolutionY = 1080;
	double layerThickness = 0.10;
	int antialiasingLevel = 8;
	bool antialiasing = true;
	float pixelPrecision = 0.1f; 
	bool xMirrored = false;
	bool yMirrored = false;
	int baseLayerCount = 5;
	int transitionLayerCount = 0;
	int transitionType = 0;
	int exposureTime = 3000; 
	int baseExposureTime = 10000; 
	int supportDelay = 3000; 
	int baseSupportDelay = 3000; 
	bool supportExposureSwitch = false;

	int transitionLayerDelay = 3000; 
	int liftingDelayBefore = 3000; 
	int liftingDelayAfter = 3000; 
	int retractDelay = 3000; 
	double baseLiftingDistanceLS = 5; 
	double baseLiftingDistance = 5; 
	double liftingDistanceLS = 5; 
	double liftingDistance = 5; 
	double baseRetractDistanceLS = 5; 
	double baseRetractDistance = 5; 
	double retractDistanceLS = 5; 
	double retractDistance = 5; 
	int    curvatureType = 0;
	double baseLiftingSpeedStart = 7.5;
	double baseLiftingSpeedLS = 10;
	double baseLiftingSpeed = 10;
	double baseLiftingCurvature = 5;
	double liftingSpeedStart = 7.5;
	double liftingSpeedLS = 10; 
	double liftingSpeed = 10;
	double liftingCurvature = 5;
	double baseRetractSpeedStart = 7.5;
	double baseRetractSpeedLS = 10;
	double baseRetractSpeed = 10;
	double baseRetractCurvature = 5;
	double retractSpeedStart = 7.5;
	double retractSpeedLS = 10;
	double retractSpeed = 10;
	double retractCurvature = 5;

	int baseLightPwm = 255;
	int lightPwm = 255;
	bool isGray = false;
	bool isDistortion = false;
};