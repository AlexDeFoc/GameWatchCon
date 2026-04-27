#include "Pch.h"
#include "TaskDispatcher.h"

int main() {
    try {
        gw::TaskDispatcher dispatcher{};
        dispatcher.Start();
    } catch (...) {
        return 0;
    }

    return 0;
}