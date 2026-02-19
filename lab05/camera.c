#include <ti/devices/msp/msp.h>
#include "lab5\camera.h"


extern uint8_t cameraData_complete;//0 = not ready, 1 = ready
extern uint16_t cameraData[128];
extern unsigned pixel_counter;

/**
 * @brief Initialize camera associated components
*/
void Camera_init(void){
	
}


/**
 * @brief Checks whether camer data is ready to retrieve
 * @note Make sure to check all data is available
 * @return True(1)/False(0) if camera data is ready
*/
uint8_t Camera_isDataReady(void){
	return cameraData_complete;
}


/**
 * @brief Retrieves pointer to camera data array
 * @return Pointer to global data array stored locally in this file
*/
uint16_t* Camera_getData(void){
	return cameraData;
}
