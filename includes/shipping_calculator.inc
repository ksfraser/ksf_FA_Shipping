<?php
/**
 * FA Shipping Module - Shipping Calculator Integration
 * 
 * This file integrates the ksf_Shipping_Core with FrontAccounting
 */

require_once __DIR__ . '/../../ksf_Shipping_Core/src/Shipping/CarrierAdapters.php';
require_once __DIR__ . '/../../ksf_Shipping_Core/src/Shipping/CanadianShippingCalculator.php';

/**
 * Initialize Canadian Shipping Calculator with configured carriers
 */
function init_shipping_calculator() {
    $calculator = new Ksf\Shipping\CanadianShippingCalculator();
    
    // Set store address from FA company setup
    $company = get_company_prefs();
    $storeAddress = [
        'company' => $company['coy_name'],
        'address' => $company['postal_address'],
        'city' => $company['city'] ?? '',
        'state' => $company['state'] ?? '',
        'postcode' => $company['postcode'] ?? '',
        'country' => $company['country'] ?? 'CA'
    ];
    $calculator->setStoreAddress($storeAddress);
    
    // Register configured carriers
    $carriers = ['canada_post', 'ups', 'fedex', 'dhl', 'purolator', 'canpar'];
    
    foreach ($carriers as $carrierName) {
        if (validate_carrier_config($carrierName)) {
            $config = get_carrier_config($carrierName);
            $adapter = create_carrier_adapter($carrierName, $config);
            if ($adapter) {
                $calculator->registerCarrier($adapter);
            }
        }
    }
    
    return $calculator;
}

/**
 * Create carrier adapter instance
 */
function create_carrier_adapter($carrier, $config) {
    switch($carrier) {
        case 'canada_post':
            return new Ksf\Shipping\CanadaPostAdapter($config);
        case 'ups':
            return new Ksf\Shipping\UPS_CanadaAdapter($config);
        case 'fedex':
            return new Ksf\Shipping\FedEx_CanadaAdapter($config);
        case 'dhl':
            return new Ksf\Shipping\DHL_CanadaAdapter($config);
        case 'purolator':
            return new Ksf\Shipping\PurolatorAdapter($config);
        case 'canpar':
            return new Ksf\Shipping\CanparAdapter($config);
        default:
            return null;
    }
}

/**
 * Calculate shipping for an order
 * 
 * @param int $order_id FA order ID
 * @return array Shipping quotes
 */
function calculate_order_shipping($order_id) {
    $order = get_sales_order($order_id);
    if (!$order) {
        return [];
    }
    
    // Get customer address
    $customer = get_customer($order['customer_id']);
    $customerAddress = [
        'company' => $customer['name'],
        'address' => $customer['address'],
        'city' => $customer['city'],
        'state' => $customer['state'],
        'postcode' => $customer['zip'],
        'country' => $customer['country'] ?? 'CA'
    ];
    
    // Calculate total weight and dimensions from order items
    $parcel = [
        'weight' => 0,
        'length' => 0,
        'width' => 0,
        'height' => 0,
        'parcel_class' => 'regular'
    ];
    
    $order_items = get_sales_order_details($order_id);
    foreach ($order_items as $item) {
        // Get product weight/dimensions (you'll need to add these fields to FA stock_master)
        $stock_item = get_stock_item($item['stock_id']);
        $qty = $item['quantity'];
        
        $parcel['weight'] += ($stock_item['weight'] ?? 0) * $qty;
        // Add logic for dimensions based on qty and packaging
    }
    
    // Get shipping quotes
    $calculator = init_shipping_calculator();
    $quotes = $calculator->getQuotes($customerAddress, $parcel);
    
    return $quotes;
}

/**
 * Display shipping quote selection in order entry
 */
function display_shipping_quote_selection($order_id) {
    $quotes = calculate_order_shipping($order_id);
    
    if (empty($quotes)) {
        display_note(_('No shipping quotes available. Please check carrier configuration.'));
        return;
    }
    
    echo "<h3>" . _('Shipping Quotes') . "</h3>";
    start_table(TABLESTYLE);
    $th = [_('Carrier'), _('Service'), _('Rate'), _('Transit Days'), _('Action')];
    table_header($th);
    
    $alt = 0;
    foreach ($quotes as $carrierName => $carrierData) {
        foreach ($carrierData['rates'] as $rate) {
            alt_table_row($alt);
            label_cell($carrierName);
            label_cell($rate['service_name']);
            amount_cell($rate['rate'], 2);
            label_cell($rate['transit_days'] . ' ' . _('days'));
            submit_cells('select_shipping_' . $rate['service_code'], _('Select'), false, _('Select this shipping method'));
            end_row();
        }
    }
    
    end_table();
}

/**
 * Save selected shipping method to order
 */
function save_order_shipping($order_id, $service_code, $rate, $carrier) {
    $sql = "UPDATE " . TB_PREF . "sales_orders SET 
            shipping_carrier=" . db_escape($carrier) . ",
            shipping_service=" . db_escape($service_code) . ",
            shipping_cost=" . (float)$rate . "
            WHERE order_no=" . (int)$order_id;
    
    db_query($sql, _('Could not update order shipping'));
}
