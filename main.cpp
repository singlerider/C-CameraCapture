// %BANNER_BEGIN%
// ---------------------------------------------------------------------
// %COPYRIGHT_BEGIN%
//
// Copyright (c) 201x Magic Leap, Inc. (COMPANY) All Rights Reserved.
// Magic Leap, Inc. Confidential and Proprietary
//
// NOTICE: All information contained herein is, and remains the property
// of COMPANY. The intellectual and technical concepts contained herein
// are proprietary to COMPANY and may be covered by U.S. and Foreign
// Patents, patents in process, and are protected by trade secret or
// copyright law. Dissemination of this information or reproduction of
// this material is strictly forbidden unless prior written permission is
// obtained from COMPANY. Access to the source code contained herein is
// hereby forbidden to anyone except current COMPANY employees, managers
// or contractors who have executed Confidentiality and Non-disclosure
// agreements explicitly covering such access.
//
// The copyright notice above does not evidence any actual or intended
// publication or disclosure of this source code, which includes
// information that is confidential and/or proprietary, and is a trade
// secret, of COMPANY. ANY REPRODUCTION, MODIFICATION, DISTRIBUTION,
// PUBLIC PERFORMANCE, OR PUBLIC DISPLAY OF OR THROUGH USE OF THIS
// SOURCE CODE WITHOUT THE EXPRESS WRITTEN CONSENT OF COMPANY IS
// STRICTLY PROHIBITED, AND IN VIOLATION OF APPLICABLE LAWS AND
// INTERNATIONAL TREATIES. THE RECEIPT OR POSSESSION OF THIS SOURCE
// CODE AND/OR RELATED INFORMATION DOES NOT CONVEY OR IMPLY ANY RIGHTS
// TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE,
// USE, OR SELL ANYTHING THAT IT MAY DESCRIBE, IN WHOLE OR IN PART.
//
// %COPYRIGHT_END%
// --------------------------------------------------------------------
// %BANNER_END%

#include <string>

#include <ml_perception.h>
#include <ml_lifecycle.h>
#include <ml_logging.h>
#include <ml_privileges.h>
#include <ml_camera.h>

int main() {
  MLLifecycleCallbacks callbacks{};
  MLLifecycleInit(&callbacks, nullptr); // Initialize Lifecycle with empty values
  MLPerceptionSettings settings; // Initialize empty MLPerceptionSettings
  MLPerceptionInitSettings(&settings);
  MLResult privilege_start_result = MLPrivilegesStartup(); // First, set up initial Privileges
  if (privilege_start_result == MLResult_Ok) {
    ML_LOG_TAG(Info, "MLPrivilegesStartup", "%s", MLGetResultString(privilege_start_result)); // Use MLGetResultString to return a std::string from MLResult
    /* Request Privilege synchronously. This will open a dialog on a user's device. If declined ONCE,
      a User must go to device Settings => Privacy and enable the permission explicitly:
    */
    MLResult privilege_request_result = MLPrivilegesRequestPrivilege(MLPrivilegeID_CameraCapture);
    if (privilege_request_result == MLPrivilegesResult_Granted) { // User Allowed CameraCapture
      ML_LOG_TAG(Info, "MLPrivilegesRequestPrivilege", "%s", MLGetResultString(privilege_request_result));
      MLLifecycleSelfInfo *lifecycle_info = nullptr; // Create empty MLLifecycleSelfInfo instance
      MLLifecycleGetSelfInfo(&lifecycle_info); // Populate lifecycle_info
      if (lifecycle_info == nullptr) { // This will have empty values if not running on a device
        ML_LOG(Fatal, "Failed to get Lifecycle info.");
      }
      const char* writable_dir_path = lifecycle_info->writable_dir_path_locked_and_unlocked; // Get MLLifecycleSelfInfo instance writable_dir_path_locked_and_unlocked
      std::string output_file_path_initial = "test_image.png"; // Set image to save's filename as "test_image.png"
      std::string output_file_path = writable_dir_path + output_file_path_initial; // "/documents/C1/test_image.png"
      MLResult camera_connect_result = MLCameraConnect(); // Connect to the camera
      ML_LOG_TAG(Info, "MLCameraConnect", "%s", MLGetResultString(camera_connect_result));
      MLHandle request_handle = ML_INVALID_HANDLE; // Create a throwaway MLHandle, to be used in MLCameraPrepareCapture call as pointer
      MLResult prepare_camera_capture_result = MLCameraPrepareCapture(MLCameraCaptureType_Image, &request_handle); // MUST prepare for camera capture
      ML_LOG_TAG(Info, "MLCameraPrepareCapture", "%s", MLGetResultString(prepare_camera_capture_result));
      MLResult capture_start_result = MLCameraCaptureImage(output_file_path.c_str()); // Save an image to the specified path
      /*
        To pull this image:
        `mldb pull -p com.magicleap.capi.sample.cameracapture /documents`
        `open documents/C1/test_image.png` 
      */
      ML_LOG_TAG(Info, "MLCameraCaptureImage", "%s", MLGetResultString(capture_start_result));
      if (capture_start_result == MLResult_Ok) { // Image captured successfully
        ML_LOG(Info, "Writing the images to %s on line %d", output_file_path.c_str(), __LINE__);
      }
      MLResult camera_disconnect_result = MLCameraDisconnect(); // Disconnect from Camera when done
      ML_LOG_TAG(Info, "MLCameraDisconnect", "%s", MLGetResultString(camera_disconnect_result));
    } else { // CameraCapture Privilege Denied by User
      ML_LOG_TAG(Info, "MLPrivilegesRequestPrivilege", "%s", MLGetResultString(privilege_request_result));
    }
  } else { // MLPrivilegesStartup Failure
    ML_LOG_TAG(Info, "MLPrivilegesStartup", "%s", MLGetResultString(privilege_start_result));
  }
  return 0;
}
