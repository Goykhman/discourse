#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <dlfcn.h>

#include "calculation.h"


constexpr double EPS = 1e-15;


struct dlclose_functor {
    void operator()(void* handle) const {
        if (handle) {
            dlclose(static_cast<void*>(handle));
        }
    }
};


using DlHandle = std::unique_ptr<void, dlclose_functor>;


DlHandle load_runtime_lib(const char* lib_path) {
    auto handle = DlHandle(dlopen(lib_path, RTLD_LAZY));
    if (!handle) {
        std::cerr << "Failed to load " << lib_path << " with error " << dlerror() << "\n";
        return nullptr;
    } else {
        std::cerr << "Loaded " << lib_path << "\n";
    }
    return handle;
}


void call_cfunc_calculation() {
    using CalculationType = void (*)(double*, double*, double);
    CalculationType calculation = cfunc_ZN8__main__11calculationB2v1B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE10float64_2a10float64_2ad;
    std::cout << "calculation address = " << reinterpret_cast<std::uintptr_t>(calculation) << "\n";
    double arr1[] = {1.2, 3.4, 5.6, 7.8, 9.1};
    double v1 = -1.4;
    double arr2[5];

    calculation(arr1, arr2, v1);

    for (int i=0; i<5; ++i) {
        auto ref = arr1[i] + v1;
        std::cout << "(" << arr2[i] << ", " << ref << "),  ";
        assert(std::abs(arr1[i] + v1 - arr2[i]) < EPS);
    }
    std::cout << "\n";
}


void load_another_calculation() {
    auto another_calculation_lib_handle = load_runtime_lib("./libanother_calculation.dylib");
    const char* another_calculation_name = "cfunc_ZN8__main__19another_calculationB2v3B38c8tJTIeFIjxB2IKSgI4CrvQClQZ6FczSBAA_3dEd";
    void* another_calculation = dlsym(another_calculation_lib_handle.get(), another_calculation_name);

    using AnotherCalculationType = double(*)(double);
    AnotherCalculationType another_calculation_func = reinterpret_cast<AnotherCalculationType>(another_calculation);
    std::cout << "another_calculation address = "
    << reinterpret_cast<std::uintptr_t>(another_calculation_func) << "\n";

    double r = 1.41;
    double area = another_calculation_func(r);
    std::cout << "area of circle with radius " << r << " is equal to " << area << "\n";
    assert(std::abs(3.141 * std::pow(r, 2) - area) < EPS);
}


int main(int argc, char* argv[]) {
    const char* nrt_runtime_lib_path = argv[1];
    auto nrt_runtime_lib_handle = load_runtime_lib(nrt_runtime_lib_path);

    call_cfunc_calculation();
    load_another_calculation();

    return 0;
}
