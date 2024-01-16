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
        <li>✅Hand recognition</li>
        <li>✅Rock-Paper-Scissors with hand recognition(Not OOP)</li>
        <li>❗Create the button UI.(OOP) -> issue</li>
        <li>❌Refactoring - RPS(OOP)</li>
        <li>❌Test Tools(CI/CD)</li>
        <li>❌Support for CMake.</li>
        <li>❌Support for Arduino.</li>
    </ul>
</details>

## Requirement

> [!NOTE]
> The versions as of developments are as below.

* Windows 11
* Logicool C922n
* OpenCV 4.5.1
* Bazel  6.0.0
    <details>
        <summary>Why use Bazel?</summary>
        Because <a href="https://developers.google.com/mediapipe">MediaPipe</a> was the first tool I learned about hand recognition.<br>
        And Bazel was used as a compiler for MediaPipe.<br>
        <p>
            <br>
            <strong>
                I plan to support CMake in the future.<br>
            </strong>
            There was a problem with embedded devices (e.g. Arduino).
        </p>
    </details>

## Getting Started

***1. Check for the execution environment.***

```bash
cd env-check
bazel build //src:test
cd bazel-bin/src
test.exe
```

* Your OpenCV version is displayed on the console.
* A live video from your webcam is displayed.(Quit : q)

***2. Check for your hand can be distinguished from the background within ROI.***

![hand-result](img/hand-result.png)

```bash
cd hand-recognition
bazel build //src:hand
cd bazel-bin/src
hand.exe
```
Adjust the HSV on the track bar so that your hand are clearly visible.

> [!CAUTION]
> If the background is close to your skin color, your hand and the background are not properly separated.

***3. Create the button UI.(OOP)***
![class](img/class.png)

## Usage

```bash
cd rps
bazel build //src:rps
cd bazel-bin/src
rps.exe
```
1. Press the "じゃんけん" button.
2. When "最初はグー" is displayed first, hold the "rock" shape for 2 to 3 seconds.
3. When "じゃんけん" is displayed, promptly play rock, paper or scissors.
4. When "ぽん！！" is displayed, and the rock-paper-scissors you played is shown in a separate window.
5. The result of the rock-paper-scissors game with the CPU is displayed on the console screen.

## To be added
![Arduino](img/Arduino.png)

* Arduino will also be supported.

---

Thank you😉