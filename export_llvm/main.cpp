#include <cassert>
#include <iostream>
#include <string>
#include <dlfcn.h>


extern "C" {
    void cfunc_ZN8__main__11calculationB2v1B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE10float64_2a10float64_2ad(double* , double* , double );
}


void* load_numba_runtime(const char* lib_path) {
    void* handle = dlopen(lib_path, RTLD_LAZY);
    if (!handle) {
        std::cerr << "Failed to load Numba runtime at " << lib_path << " with error " << dlerror() << "\n";
        return nullptr;
    } else {
        std::cerr << "Loaded Numba runtime at " << lib_path << "\n";
    }
    return handle;
}


void call_cfunc_calculation() {
    using FuncType = void (*)(double*, double*, double);
    FuncType calculation = cfunc_ZN8__main__11calculationB2v1B52c8tJTIeFIjxB2IKSgI4CrvQClUYkACQB1EiFSRRB9GgCAA_3d_3dE10float64_2a10float64_2ad;
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


int main(int argc, char* argv[]) {
    const char* nrt_runtime_lib_path = argv[1];
    void* nrt_runtime_lib_handle = load_numba_runtime(nrt_runtime_lib_path);
    call_cfunc_calculation();
    dlclose(nrt_runtime_lib_handle);
    return 0;
}
