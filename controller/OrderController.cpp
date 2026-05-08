#include "OrderController.h"
#include "../view/OrderView.h"

OrderController::OrderController(SampleRepository& sampleRepo, OrderRepository& orderRepo)
    : sampleRepo_(sampleRepo), orderRepo_(orderRepo) {}

void OrderController::run() {
    int choice;
    do {
        choice = OrderView::readMenuChoice();
        switch (choice) {
        case 1: registerOrder();  break;
        case 2: listOrders();     break;
        case 3: processRelease(); break;
        case 0: break;
        default: OrderView::showError("잘못된 입력입니다."); break;
        }
    } while (choice != 0);
}

void OrderController::registerOrder() {
    OrderView::showSamples(sampleRepo_.getAll());
    int sampleId = OrderView::readSampleId();
    if (!sampleRepo_.findById(sampleId)) {
        OrderView::showError("존재하지 않는 시료입니다.");
        return;
    }
    int quantity = OrderView::readQuantity();
    if (quantity < 1) {
        OrderView::showError("수량은 1 이상이어야 합니다.");
        return;
    }
    Order o{ 0, sampleId, quantity, OrderStatus::RESERVED };
    orderRepo_.add(o);
    OrderView::showOrderRegistered(o);
}

void OrderController::listOrders() {
    OrderView::showOrders(orderRepo_.getAll(), sampleRepo_.getAll());
}

void OrderController::processRelease() {
    bool any = false;
    for (const auto& o : orderRepo_.getAll())
        if (o.status == OrderStatus::CONFIRMED) { any = true; break; }
    if (!any) {
        OrderView::showError("출고 대기 중인 주문이 없습니다.");
        return;
    }
    OrderView::showConfirmedOrders(orderRepo_.getAll(), sampleRepo_.getAll());
    int id = OrderView::readOrderId();
    Order* o = orderRepo_.findById(id);
    if (!o || o->status != OrderStatus::CONFIRMED) {
        OrderView::showError("유효하지 않은 주문 ID입니다.");
        return;
    }
    o->status = OrderStatus::RELEASED;
    orderRepo_.save();
    OrderView::showReleased(*o);
}
