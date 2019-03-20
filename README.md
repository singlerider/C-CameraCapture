# Camera Capture for Magic Leap C API

This codebase covers the following topics:

* [MLLifecycleSelfInfo](https://developer-documentation.magicleap.com/public/v0.19.0/CAPI/group___lifecycle.html#ga871dbc43b4eb25abe4766eaf35d92049)
* [MLPrivilegesStartup](https://developer-documentation.magicleap.com/public/v0.19.0/CAPI/group___privileges.html#gabdf1a6912c0c8c52fd110771cec63191)
* [MLGetResultString](https://developer-documentation.magicleap.com/public/v0.19.0/UnityAPI/class_magic_leap_internal_1_1_magic_leap_native_bindings.html#a6cdbb2a6e6d4e721cb5e4fa429a838f0)
* CameraCapture flow:
    1. [MLPrivilegesRequestPrivilege](https://developer-documentation.magicleap.com/public/v0.19.0/CAPI/group___privileges.html#ga44c6ae3af0b8be953bb092764947663d)
    2. [MLLifecycleSelfInfo->writable_dir_path_locked_and_unlocked](https://developer-documentation.magicleap.com/public/v0.19.0/CAPI/group___lifecycle.html#struct_m_l_lifecycle_self_info)
    3. [MLCameraConnect](https://developer-documentation.magicleap.com/public/v0.19.0/CAPI/group___camera.html#ga93e7ffc829407c25c2483c5889e5e1a3)
    4. [MLCameraPrepareCapture](https://developer-documentation.magicleap.com/public/v0.19.0/CAPI/group___camera.html#gaa358f3ed755860680c70a3dc6361a080)
    5. [MLCameraCaptureImage](https://developer-documentation.magicleap.com/public/v0.19.0/CAPI/group___camera.html#gaa358f3ed755860680c70a3dc6361a080)
    6. [MLCameraDisconnect](https://developer-documentation.magicleap.com/public/v0.19.0/CAPI/group___camera.html#ga790b047c1ec955126722075b10301f29)
* Pull files from the device from persistent app storage

## Machine Configuration
First, start by reading about getting Visual Studio/Visual Studio Code set up for C projects:

<https://creator.magicleap.com/learn/guides/programming-guide>

## Build
Once you know which application to use for compiling, build an mpk. Keep in mind, it's possible to build a version for debugging and for release. Choose a `Debug` build for now.

## Install
Locate the built mpk, then execute:

```shell
$ mldb install com.magicleap.capi.sample.cameracapture.mpk
```

## Launch
Since you've built a `Debug` application, we can `tail` the logs and see what the app does at every step. In a terminal window, run:

```shell
$ mldb log
```

While that is running, in a separate terminal window, execute:

```shell
$ mldb launch com.magicleap.capi.sample.cameracapture
```

Although the app is declared as a `Fullscreen` app, it only runs for a few seconds and no content is actually displayed. The first time the app is launched, it will ask for permission to access the Camera. Allow this permission. If declined, you must go to device Settings => Privacy and enable the permission explicitly.

Shortly after accepting, the device will take a single picture. This will be saved in the applicaiton's persistent storage directory associated with its package name. In this app, the directory that is saved to is `/documents/C1/`.

## Pull Files
To get the directories and images onto your development machine for inspection purposes, execute the following from any directory on the host machine:

```shell
$ mldb pull -p com.magicleap.capi.sample.cameracapture /documents
```

This will download the directory, `documents` and place it in the directory the previous command was called in.

The image that was taken by the app is `documents/C1/test_image.png`.

To learn more about working with the filesystem, see:

<https://creator.magicleap.com/learn/guides/sdk-mldb-working-with-the-file-system>
