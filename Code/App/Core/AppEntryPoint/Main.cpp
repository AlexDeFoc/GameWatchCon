#include "Main.hpp"

#include <chrono>
#include <iostream>

int main() {
    App app{};

    auto start = std::chrono::high_resolution_clock::now();
    for (int i{0}; i < 100; ++i)
        app.Start();
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    // 5. Print it out
    std::cout << "------------------------------------" << std::endl;
    std::cout << "Task creation took: " << duration.count() << " nanoseconds" << std::endl;
    std::cout << "------------------------------------" << std::endl;

    // 6. CRITICAL: Pause so the window doesn't vanish!
    std::cout << "Press Enter to close..." << std::endl;
    std::cin.get();

    return 0;
}