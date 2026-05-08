#include "ProductionController.h"
#include "../view/ProductionView.h"

ProductionController::ProductionController(SampleRepository& sampleRepo, OrderRepository& orderRepo)
    : sampleRepo_(sampleRepo), orderRepo_(orderRepo) {}

void ProductionController::run() {
    int choice;
    do {
        choice = ProductionView::readMenuChoice();
        switch (choice) {
        case 1: registerSample();     break;
        case 2: listSamples();        break;
        case 3: approveOrder();       break;
        case 4: rejectOrder();        break;
        case 5: listProduction();     break;
        case 6: completeProduction(); break;
        case 0: break;
        default: ProductionView::showError("잘못된 입력입니다."); break;
        }
    } while (choice != 0);
}

void ProductionController::registerSample() {
    std::string name  = ProductionView::readSampleName();
    int         stock = ProductionView::readInitialStock();
    if (stock < 0) {
        ProductionView::showError("재고는 0 이상이어야 합니다.");
        return;
    }
    Sample s{ 0, name, stock };
    sampleRepo_.add(s);
    ProductionView::showSampleRegistered(s);
}

void ProductionController::listSamples() {
    ProductionView::showSamples(sampleRepo_.getAll());
}

void ProductionController::approveOrder() {
    bool any = false;
    for (const auto& o : orderRepo_.getAll())
        if (o.status == OrderStatus::RESERVED) { any = true; break; }
    if (!any) {
        ProductionView::showError("승인 대기 중인 주문이 없습니다.");
        return;
    }
    ProductionView::showReservedOrders(orderRepo_.getAll(), sampleRepo_.getAll());
    int id = ProductionView::readOrderId();
    Order* o = orderRepo_.findById(id);
    if (!o || o->status != OrderStatus::RESERVED) {
        ProductionView::showError("유효하지 않은 주문 ID입니다.");
        return;
    }
    Sample* s = sampleRepo_.findById(o->sampleId);
    if (!s) {
        ProductionView::showError("시료를 찾을 수 없습니다.");
        return;
    }
    if (s->stock >= o->quantity) {
        s->stock -= o->quantity;
        o->status = OrderStatus::CONFIRMED;
        sampleRepo_.save();
    } else {
        o->status = OrderStatus::PRODUCING;
    }
    orderRepo_.save();
    ProductionView::showApproved(*o);
}

void ProductionController::rejectOrder() {
    bool any = false;
    for (const auto& o : orderRepo_.getAll())
        if (o.status == OrderStatus::RESERVED) { any = true; break; }
    if (!any) {
        ProductionView::showError("거절할 주문이 없습니다.");
        return;
    }
    ProductionView::showReservedOrders(orderRepo_.getAll(), sampleRepo_.getAll());
    int id = ProductionView::readOrderId();
    Order* o = orderRepo_.findById(id);
    if (!o || o->status != OrderStatus::RESERVED) {
        ProductionView::showError("유효하지 않은 주문 ID입니다.");
        return;
    }
    o->status = OrderStatus::REJECTED;
    orderRepo_.save();
    ProductionView::showRejected(*o);
}

void ProductionController::listProduction() {
    ProductionView::showProducingOrders(orderRepo_.getAll(), sampleRepo_.getAll());
}

void ProductionController::completeProduction() {
    bool any = false;
    for (const auto& o : orderRepo_.getAll())
        if (o.status == OrderStatus::PRODUCING) { any = true; break; }
    if (!any) {
        ProductionView::showError("생산 중인 주문이 없습니다.");
        return;
    }
    ProductionView::showProducingOrders(orderRepo_.getAll(), sampleRepo_.getAll());
    int id = ProductionView::readOrderId();
    Order* o = orderRepo_.findById(id);
    if (!o || o->status != OrderStatus::PRODUCING) {
        ProductionView::showError("유효하지 않은 주문 ID입니다.");
        return;
    }
    o->status = OrderStatus::CONFIRMED;
    orderRepo_.save();
    ProductionView::showCompleted(*o);
}
