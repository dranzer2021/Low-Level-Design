#ifndef NOW_ORDER_FACTORY_H
#define NOW_ORDER_FACTORY_H

#include"OrderFactory.h"
#include"../models/DeliveryOrder.h"
#include"../models/PickupOrder.h"
#include"../utils/TimeUtils.h"

class NowOrderFactory : public OrderFactory{
public:
     Order* createOrder(User* user, Cart* cart, Restaurant* restaurant, 
        const vector<MenuItem>& menuItems, PaymentStrategy* paymentStrategy, 
        double totalCost, const string& orderType) override {
        Order* order = nullptr;
        order->setUser(user);
        order->setRestaurant(restaurant);
        order->setItem(menuItems);
        order->setPaymentStrategy(paymentStrategy);
        order->setTotal(totalCost);
        order->setScheduled(TimeUtils::getCurrentTime());
        if(orderType == "Delivery") {
            auto deliveryOrder = new DeliveryOrder();
            deliveryOrder->setUserAddress(user->getAddress());
            order = deliveryOrder;
        }
        else {
            auto pickupOrder = new PickupOrder();
            pickupOrder->setRestaurantAddress(restaurant->getLoc());
            order = pickupOrder;
        }

        return order;
    }
    
};

#endif