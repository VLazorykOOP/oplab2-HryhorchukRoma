#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
#include <chrono>


std::mutex mtx;

class Karp {
public:
    Karp(double v, double startX, double endX) : speed(v), x(startX), startX(startX), endX(endX), direction(1) {}

    void update(double timeStep) {
        x += speed * direction * timeStep;
        if (x >= endX || x <= startX) {
            direction *= -1; // Change direction when reaching the end
        }
    }

    void printPosition() const {
        std::lock_guard<std::mutex> guard(mtx);
        std::cout << "Karp Position: (" << x << ", 0)\n";
    }

private:
    double x;
    double speed;
    double startX, endX;
    int direction;
};

class Schuka {
public:
    Schuka(double v, double startY, double endY) : speed(v), y(startY), startY(startY), endY(endY), direction(1) {}

    void update(double timeStep) {
        y += speed * direction * timeStep;
        if (y >= endY || y <= startY) {
            direction *= -1; // Change direction when reaching the end
        }
    }

    void printPosition() const {
        std::lock_guard<std::mutex> guard(mtx);
        std::cout << "Schuka Position: (0, " << y << ")\n";
    }

private:
    double y;
    double speed;
    double startY, endY;
    int direction;
};

void KarpThread(Karp& fish, double timeStep, double totalTime) {
    for (double time = 0; time <= totalTime; time += timeStep) {
        fish.update(timeStep);
        fish.printPosition();
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(timeStep * 1000)));
    }
}

void SchukaThread(Schuka& fish, double timeStep, double totalTime) {
    for (double time = 0; time <= totalTime; time += timeStep) {
        fish.update(timeStep);
        fish.printPosition();
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(timeStep * 1000)));
    }
}

int main() {
    double speed = 1.0;
    double startX = 0.0;
    double endX = 10.0;
    double startY = 0.0;
    double endY = 10.0;
    double timeStep = 1.0;
    double totalTime = 20.0;

    Karp Karp(speed, startX, endX);
    Schuka Schuka(speed, startY, endY);

    std::thread KarpT(KarpThread, std::ref(Karp), timeStep, totalTime);
    std::thread SchukaT(SchukaThread, std::ref(Schuka), timeStep, totalTime);

    KarpT.join();
    SchukaT.join();

    return 0;
}