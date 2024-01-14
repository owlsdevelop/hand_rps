# Hand_RPS

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

Hand recognition and Rock-Paper-Scissors application using webcam + OpenCV.

![Demo](https://github.com/owlsdevelop/hand_rps/assets/75362440/caa4b06b-017e-4b59-89d8-ab68eeb48657)

## Purpose

Simplify hand recognition and apply it to embedded devices and augmented reality (AR).

<details>
    <summary><b>Goals</b></summary>
    <ul>
        <li>✅Check for the execution environment.</li>
        <li>❌Create the button UI.</li>
    </ul>
</details>

## Requirement

> [!NOTE]
> The version as of developments are as below.

* Windows 11
* Logicool C922n
* OpenCV 4.5.1
* Bazel  6.0.0
    <details>
        <summary>Why use Bazel?</summary>
        Because [MediaPipe](https://developers.google.com/mediapipe) was the first tool I learned about hand recognition.  
        And Bazel was used as a compiler for MediaPipe.<br>
        <p>
            <br>
            <strong>
                I plan to support CMake in the future.<br>
            </strong>
            Because there was a problem with embedded devices (e.g. Arduino).
        </p>
    </details>

## Getting Started

1. Check for the execution environment.(env-check)

```bash
cd env-check
bazel build //src:test
cd bazel-bin/src
test.exe
```

* Your OpenCV version is displayed on the console.
* A live video from your webcam is displayed.(Quit : q)

---

Thank you:wink: