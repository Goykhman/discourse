from numba import carray, cfunc, njit
from numba.core.types import CPointer, float64, void
from numpy import float64 as float64_t


float64_p = CPointer(float64)


calculation_signature = void(float64_p, float64_p, float64)
shape = (5, )


@cfunc(calculation_signature)
def calculation(in_arr_p, out_arr_p, param):
    """
    :param in_arr_p: pointer to input array of doubles
    :param out_arr_p: pointer to output array of doubles
    :param param: use this parameter in calculation
    :return: None, change `out_arr` in-place
    """
    arr = carray(in_arr_p, shape, dtype=float64_t)
    arr2 = carray(out_arr_p, shape, dtype=float64_t)
    for i in range(shape[0]):
        arr2[i] = arr[i] + param


another_calculation_signature = float64(float64)


@njit(another_calculation_signature)
def another_calculation(param):
    return 3.141 * param ** 2


if __name__ == '__main__':
    calculation_llvm = calculation.inspect_llvm()
    calculation_llvm = calculation_llvm.replace('cfunc.', 'cfunc')

    with open('calculation.ll', 'w') as f:
        print(calculation_llvm, file=f)

    another_calculation_llvm = next(iter(another_calculation.inspect_llvm().values()))
    another_calculation_llvm = another_calculation_llvm.replace('cfunc.', 'cfunc')

    with open('another_calculation.ll', 'w') as f:
        print(another_calculation_llvm, file=f)
