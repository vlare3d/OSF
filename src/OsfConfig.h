#pragma once
//切片参数
struct OsfConfig
{
    double envelopX = 192.0;
    double envelopY = 108.0;
    double envelopZ = 200.0;
    int resolutionX = 1920;
    int resolutionY = 1080;
    double layerThickness = 0.1;
    int antialiasingLevel = 8;
    bool antialiasing = true;
    double pixelPrecision = 0.1f; // mm
    bool xMirrored = false;
    bool yMirrored = false;
    int baseLayerCount = 5;
    int transitionLayerCount = 0;
    int transitionType = 0;
    int exposureTime = 3000; //ms
    int baseExposureTime = 10000; //ms
    int supportDelay = 3000; //ms
    int baseSupportDelay = 3000; //ms
    bool supportExposureSwitch = false;

    int transitionLayerDelay = 3000; //ms
    int baseLiftingDelayBefore = 3000; //ms
    int baseLiftingDelayAfter = 3000; //ms
    int baseRetractDelay = 3000; //ms
    int liftingDelayBefore = 3000; //ms
    int liftingDelayAfter = 3000; //ms
    int retractDelay = 3000; //ms
    double baseLiftingDistanceLS = 5; // mm
    double baseLiftingDistance = 5; // mm
    double liftingDistanceLS = 5; // mm
    double liftingDistance = 5; // mm
    double baseRetractDistanceLS = 5; // mm
    double baseRetractDistance = 5; // mm
    double retractDistanceLS = 5; // mm
    double retractDistance = 5; // mm
    int    curvatureType = 0;
    double baseLiftingSpeedStart = 7.5;
    double baseLiftingSpeedLS = 10;// mm/min
    double baseLiftingSpeed = 10;// mm/min
    double baseLiftingSpeedEnd = 7;
    double baseLiftingCurvature = 5;
    double baseLiftingCurvatureDec = 5;
    double liftingSpeedStart = 7.5;
    double liftingSpeedLS = 10; // mm/min
    double liftingSpeed = 10;// mm/min
    double liftingSpeedEnd = 7;
    double liftingCurvature = 5;
    double liftingCurvatureDec = 5;
    double baseRetractSpeedStart = 7.5;
    double baseRetractSpeedLS = 10;// mm/min
    double baseRetractSpeed = 10;// mm/min
    double baseRetractSpeedEnd = 7;
    double baseRetractCurvature = 5;
    double baseRetractCurvatureDec = 5;
    double retractSpeedStart = 7.5;
    double retractSpeedLS = 10;// mm/min
    double retractSpeed = 10;// mm/min
    double retractSpeedEnd = 7;
    double retractCurvature = 5;
    double retractCurvatureDec = 5;

    int baseLightPwm = 255;
    int lightPwm = 255;
    bool isGray = false;
    bool isDistortion = false;
};
