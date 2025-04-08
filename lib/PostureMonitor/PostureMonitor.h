#ifndef POSTURE_MONITOR_H
#define POSTURE_MONITOR_H

#include "MPU6050Handler.h"
#include "FeedbackHandler.h"
#include "Logger.h"

class PostureMonitor {
public:
    void initialize();
    void update(); // Agora totalmente não-bloqueante

private:
    MPU6050Handler mpuHandler;
    FeedbackHandler feedbackHandler;
    
    unsigned long lastFeedbackTime = 0;
    const unsigned long feedbackCooldown = 30000; // 30 segundos
    PostureState currentPosture = POSTURE_GOOD;
    unsigned long postureTimer = 0;
    float pitch = 0, roll = 0;
    
    void handlePostureState(PostureState state, bool stateChanged);
    const char* getActionMessage(PostureState state);
};

#endif