#ifndef BACKEND_H
#define BACKEND_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <pthread.h>

class backend
{
public:
    backend();
    struct grabber_t
    {
        long thread_id;
        int device_id;
        int capture_id;
        std::string stream_location;
    };

    vector<grabber_t> gabber_ts;

    void *grabber(void* threadarg)
    {
        struct grabber_t *data;
        data = (struct grabber_t *) threadarg;
    }

    void createGrabber(int device_id, int capture_id, std::string stream_location)
    {
        pthread_t threads[0];
        struct grabber_t grabber_data;
        grabber_data.capture_id = capture_id;
        grabber_data.device_id = device_id;
        grabber_data.stream_location = stream_location;

        int rc = pthread_create(&threads[0], NULL, grabber, (void *) grabber_data);
    }
};

#endif // BACKEND_H
