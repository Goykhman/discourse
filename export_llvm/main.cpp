#include <cassert>
#include <iostream>
#include <string>
#include <dlfcn.h>


extern "C" {
    void cfunc_ZN8__main__11calculationB2v1B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE10float64_2a10float64_2ad(double* , double* , double );
}


void* load_runtime_lib(const char* lib_path) {
    void* handle = dlopen(lib_path, RTLD_LAZY);
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
    std::cout << "calculation address = " << (std::uintptr_t)calculation << "\n";
    double arr1[] = {1.2, 3.4, 5.6, 7.8, 9.1};
    double v1 = -1.4;
    double arr2[5];

    calculation(arr1, arr2, v1);

    for (int i=0; i<5; ++i) {
        auto ref = arr1[i] + v1;
        std::cout << "(" << arr2[i] << ", " << ref << "),  ";
        assert(arr2[i] == arr1[i] + v1);
    }
    std::cout << "\n";
}


void load_another_calculation() {
    void* another_calculation_lib_handle = load_runtime_lib("./libanother_calculation.dylib");
    const char* another_calculation_name = "cfunc_ZN8__main__19another_calculationB2v3B38c8tJTIeFIjxB2IKSgI4CrvQClQZ6FczSBAA_3dEd";
    void* another_calculation = dlsym(another_calculation_lib_handle, another_calculation_name);

    using AnotherCalculationType = double(*)(double);
    AnotherCalculationType another_calculation_func = (AnotherCalculationType)another_calculation;
    std::cout << "another_calculation address = " << (std::uintptr_t)another_calculation_func << "\n";

    double r = 1.41;
    double area = another_calculation_func(r);
    std::cout << "area of circle with radius " << r << " is equal to " << area << "\n";
    assert((3.141 * r * r - area) < 1e-15);
    dlclose(another_calculation_lib_handle);
}


int main(int argc, char* argv[]) {
    const char* nrt_runtime_lib_path = argv[1];
    void* nrt_runtime_lib_handle = load_runtime_lib(nrt_runtime_lib_path);

    call_cfunc_calculation();
    dlclose(nrt_runtime_lib_handle);

    load_another_calculation();

    return 0;
}
