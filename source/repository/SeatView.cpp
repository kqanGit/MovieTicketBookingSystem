#include "SeatView.h"

SeatView::SeatView(std::shared_ptr<ISeat> seat, const SeatStatus& status) : seat(seat), status(status) {}