#include "OrderView.h"
#include <iostream>
#include <iomanip>

int OrderView::readMenuChoice() {
    std::cout << "\n=== 주문 담당자 메뉴 ===\n"
              << "1. 주문 등록\n"
              << "2. 주문 목록 조회\n"
              << "3. 출고 처리\n"
              << "0. 역할 재선택\n"
              << "선택: ";
    int c; std::cin >> c;
    return c;
}

void OrderView::showSamples(const std::vector<Sample>& samples) {
    std::cout << "\n[등록된 시료]\n"
              << std::left << std::setw(6) << "ID" << std::setw(20) << "이름" << "재고\n"
              << std::string(32, '-') << '\n';
    if (samples.empty()) { std::cout << "(등록된 시료 없음)\n"; return; }
    for (const auto& s : samples)
        std::cout << std::setw(6) << s.id << std::setw(20) << s.name << s.stock << '\n';
}

int OrderView::readSampleId() {
    std::cout << "시료 ID: ";
    int v; std::cin >> v; return v;
}

int OrderView::readQuantity() {
    std::cout << "수량: ";
    int v; std::cin >> v; return v;
}

void OrderView::showOrders(const std::vector<Order>& orders, const std::vector<Sample>& samples) {
    auto name = [&](int id) {
        for (const auto& s : samples) if (s.id == id) return s.name;
        return std::string("?");
    };
    std::cout << "\n[주문 목록]\n"
              << std::left << std::setw(6) << "ID" << std::setw(16) << "시료"
              << std::setw(8) << "수량" << "상태\n"
              << std::string(44, '-') << '\n';
    bool any = false;
    for (const auto& o : orders) {
        if (o.status == OrderStatus::REJECTED) continue;
        std::cout << std::setw(6) << o.id << std::setw(16) << name(o.sampleId)
                  << std::setw(8) << o.quantity << toString(o.status) << '\n';
        any = true;
    }
    if (!any) std::cout << "(주문 없음)\n";
}

void OrderView::showConfirmedOrders(const std::vector<Order>& orders, const std::vector<Sample>& samples) {
    auto name = [&](int id) {
        for (const auto& s : samples) if (s.id == id) return s.name;
        return std::string("?");
    };
    std::cout << "\n[출고 대기 목록]\n"
              << std::left << std::setw(6) << "ID" << std::setw(16) << "시료" << "수량\n"
              << std::string(28, '-') << '\n';
    for (const auto& o : orders) {
        if (o.status != OrderStatus::CONFIRMED) continue;
        std::cout << std::setw(6) << o.id << std::setw(16) << name(o.sampleId) << o.quantity << '\n';
    }
}

int OrderView::readOrderId() {
    std::cout << "주문 ID: ";
    int v; std::cin >> v; return v;
}

void OrderView::showOrderRegistered(const Order& o) {
    std::cout << "[완료] 주문 등록 — ID: " << o.id
              << ", 시료ID: " << o.sampleId << ", 수량: " << o.quantity << '\n';
}

void OrderView::showReleased(const Order& o) {
    std::cout << "[완료] 주문 #" << o.id << " 출고 완료\n";
}

void OrderView::showError(const std::string& msg) {
    std::cout << "[오류] " << msg << '\n';
}
