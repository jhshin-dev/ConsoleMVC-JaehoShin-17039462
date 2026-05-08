#include <iostream>
#include <windows.h>
#include "model/SampleRepository.h"
#include "model/OrderRepository.h"
#include "controller/OrderController.h"
#include "controller/ProductionController.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    SampleRepository     sampleRepo;
    OrderRepository      orderRepo;
    OrderController      orderCtrl(sampleRepo, orderRepo);
    ProductionController prodCtrl(sampleRepo, orderRepo);

    int role;
    do {
        std::cout << "\n=== S-Semi 시료 생산주문관리 시스템 ===\n"
                  << "1. 주문 담당자\n"
                  << "2. 생산 담당자\n"
                  << "0. 종료\n"
                  << "역할 선택: ";
        std::cin >> role;
        switch (role) {
        case 1: orderCtrl.run(); break;
        case 2: prodCtrl.run();  break;
        case 0: std::cout << "시스템을 종료합니다.\n"; break;
        default: std::cout << "[오류] 잘못된 입력입니다.\n"; break;
        }
    } while (role != 0);

    return 0;
}
