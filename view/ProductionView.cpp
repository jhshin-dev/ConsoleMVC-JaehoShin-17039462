#include "ProductionView.h"
#include <iostream>
#include <iomanip>

int ProductionView::readMenuChoice() {
    std::cout << "\n=== 생산 담당자 메뉴 ===\n"
              << "1. 시료 등록\n"
              << "2. 시료 목록 조회\n"
              << "3. 주문 승인\n"
              << "4. 주문 거절\n"
              << "5. 생산 현황 조회\n"
              << "6. 생산 완료 처리\n"
              << "0. 역할 재선택\n"
              << "선택: ";
    int c; std::cin >> c;
    return c;
}

std::string ProductionView::readSampleName() {
    std::cout << "시료 이름: ";
    std::string v; std::cin >> v; return v;
}

int ProductionView::readInitialStock() {
    std::cout << "초기 재고: ";
    int v; std::cin >> v; return v;
}

void ProductionView::showSamples(const std::vector<Sample>& samples) {
    std::cout << "\n[시료 목록]\n"
              << std::left << std::setw(6) << "ID" << std::setw(20) << "이름" << "재고\n"
              << std::string(32, '-') << '\n';
    if (samples.empty()) { std::cout << "(등록된 시료 없음)\n"; return; }
    for (const auto& s : samples)
        std::cout << std::setw(6) << s.id << std::setw(20) << s.name << s.stock << '\n';
}

void ProductionView::showReservedOrders(const std::vector<Order>& orders, const std::vector<Sample>& samples) {
    auto name = [&](int id) {
        for (const auto& s : samples) if (s.id == id) return s.name;
        return std::string("?");
    };
    std::cout << "\n[승인 대기 주문]\n"
              << std::left << std::setw(6) << "ID" << std::setw(16) << "시료" << "수량\n"
              << std::string(28, '-') << '\n';
    for (const auto& o : orders) {
        if (o.status != OrderStatus::RESERVED) continue;
        std::cout << std::setw(6) << o.id << std::setw(16) << name(o.sampleId) << o.quantity << '\n';
    }
}

void ProductionView::showProducingOrders(const std::vector<Order>& orders, const std::vector<Sample>& samples) {
    auto name = [&](int id) {
        for (const auto& s : samples) if (s.id == id) return s.name;
        return std::string("?");
    };
    std::cout << "\n[생산 현황 - PRODUCING]\n"
              << std::left << std::setw(6) << "ID" << std::setw(16) << "시료" << "수량\n"
              << std::string(28, '-') << '\n';
    bool any = false;
    for (const auto& o : orders) {
        if (o.status != OrderStatus::PRODUCING) continue;
        std::cout << std::setw(6) << o.id << std::setw(16) << name(o.sampleId) << o.quantity << '\n';
        any = true;
    }
    if (!any) std::cout << "(생산 중인 주문 없음)\n";
}

int ProductionView::readOrderId() {
    std::cout << "주문 ID: ";
    int v; std::cin >> v; return v;
}

void ProductionView::showSampleRegistered(const Sample& s) {
    std::cout << "[완료] 시료 등록 — ID: " << s.id << ", 이름: " << s.name << ", 재고: " << s.stock << '\n';
}

void ProductionView::showApproved(const Order& o) {
    std::cout << "[완료] 주문 #" << o.id << " 승인 → " << toString(o.status) << '\n';
}

void ProductionView::showRejected(const Order& o) {
    std::cout << "[완료] 주문 #" << o.id << " 거절\n";
}

void ProductionView::showCompleted(const Order& o) {
    std::cout << "[완료] 주문 #" << o.id << " 생산 완료 → CONFIRMED\n";
}

void ProductionView::showError(const std::string& msg) {
    std::cout << "[오류] " << msg << '\n';
}
