<?php
/**
 * FA Shipping Module - Carrier Configuration UI
 * 
 * This file defines the admin UI for configuring Canadian carrier API credentials
 */

/**
 * Display carrier configuration form
 * 
 * @param string $carrier Carrier name (canada_post, ups, fedex, dhl, purolator, canpar)
 */
function display_carrier_config_form($carrier) {
    $config = get_carrier_config($carrier);
    
    start_form(true);
    table_start();
    
    switch($carrier) {
        case 'canada_post':
            display_canada_post_config($config);
            break;
        case 'ups':
            display_ups_config($config);
            break;
        case 'fedex':
            display_fedex_config($config);
            break;
        case 'dhl':
            display_dhl_config($config);
            break;
        case 'purolator':
            display_purolator_config($config);
            break;
        case 'canpar':
            display_canpar_config($config);
            break;
    }
    
    table_end();
    
    submit_center('save', _('Save Configuration'));
    end_form();
}

/**
 * Canada Post Configuration
 */
function display_canada_post_config($config) {
    table_section_title(_('Canada Post API Configuration'));
    
    text_row_ex(_('API Key'), 'canada_post_api_key', 50, 100, $config['api_key'] ?? '');
    text_row_ex(_('Customer Number'), 'canada_post_customer_number', 20, 20, $config['customer_number'] ?? '');
    yesno_list_row(_('Enable Test Mode'), 'canada_post_test_mode', $config['test_mode'] ?? 0);
    text_row_ex(_('Contract ID (Optional)'), 'canada_post_contract_id', 30, 50, $config['contract_id'] ?? '');
}

/**
 * UPS Configuration
 */
function display_ups_config($config) {
    table_section_title(_('UPS Canada OAuth/API Configuration'));
    
    text_row_ex(_('OAuth Token'), 'ups_oauth_token', 60, 200, $config['oauth_token'] ?? '', false, false, true);
    text_row_ex(_('Shipper Number'), 'ups_shipper_number', 20, 20, $config['shipper_number'] ?? '');
    text_row_ex(_('Access License Number'), 'ups_access_license', 30, 50, $config['access_license'] ?? '');
    yesno_list_row(_('Enable Test Mode'), 'ups_test_mode', $config['test_mode'] ?? 0);
}

/**
 * FedEx Configuration
 */
function display_fedex_config($config) {
    table_section_title(_('FedEx Canada OAuth/API Configuration'));
    
    text_row_ex(_('Client ID'), 'fedex_client_id', 50, 100, $config['client_id'] ?? '');
    text_row_ex(_('Client Secret'), 'fedex_client_secret', 50, 100, $config['client_secret'] ?? '', false, false, true);
    text_row_ex(_('Account Number'), 'fedex_account_number', 20, 20, $config['account_number'] ?? '');
    yesno_list_row(_('Enable Test Mode'), 'fedex_test_mode', $config['test_mode'] ?? 0);
}

/**
 * DHL Configuration
 */
function display_dhl_config($config) {
    table_section_title(_('DHL Canada API Configuration'));
    
    text_row_ex(_('API Key'), 'dhl_api_key', 60, 200, $config['api_key'] ?? '', false, false, true);
    text_row_ex(_('Account Number'), 'dhl_account_number', 20, 20, $config['account_number'] ?? '');
    yesno_list_row(_('Enable Test Mode'), 'dhl_test_mode', $config['test_mode'] ?? 0);
}

/**
 * Purolator Configuration
 */
function display_purolator_config($config) {
    table_section_title(_('Purolator API Configuration'));
    
    text_row_ex(_('API Key'), 'purolator_api_key', 50, 100, $config['api_key'] ?? '');
    text_row_ex(_('Activation Key'), 'purolator_activation_key', 50, 100, $config['activation_key'] ?? '');
    text_row_ex(_('Account Number'), 'purolator_account_number', 20, 20, $config['account_number'] ?? '');
    yesno_list_row(_('Enable Test Mode'), 'purolator_test_mode', $config['test_mode'] ?? 0);
}

/**
 * Canpar Configuration
 */
function display_canpar_config($config) {
    table_section_title(_('Canpar API Configuration'));
    
    text_row_ex(_('API Key'), 'canpar_api_key', 50, 100, $config['api_key'] ?? '');
    text_row_ex(_('Username'), 'canpar_username', 20, 50, $config['username'] ?? '');
    password_row(_('Password'), 'canpar_password', $config['password'] ?? '');
    yesno_list_row(_('Enable Guest Quotes'), 'canpar_guest_quotes', $config['guest_quotes'] ?? 1);
}

/**
 * Save carrier configuration
 */
function save_carrier_config($carrier) {
    $config = [];
    
    switch($carrier) {
        case 'canada_post':
            $config = [
                'api_key' => get_post('canada_post_api_key', ''),
                'customer_number' => get_post('canada_post_customer_number', ''),
                'test_mode' => get_post('canada_post_test_mode', 0),
                'contract_id' => get_post('canada_post_contract_id', '')
            ];
            break;
        case 'ups':
            $config = [
                'oauth_token' => get_post('ups_oauth_token', ''),
                'shipper_number' => get_post('ups_shipper_number', ''),
                'access_license' => get_post('ups_access_license', ''),
                'test_mode' => get_post('ups_test_mode', 0)
            ];
            break;
        case 'fedex':
            $config = [
                'client_id' => get_post('fedex_client_id', ''),
                'client_secret' => get_post('fedex_client_secret', ''),
                'account_number' => get_post('fedex_account_number', ''),
                'test_mode' => get_post('fedex_test_mode', 0)
            ];
            break;
        case 'dhl':
            $config = [
                'api_key' => get_post('dhl_api_key', ''),
                'account_number' => get_post('dhl_account_number', ''),
                'test_mode' => get_post('dhl_test_mode', 0)
            ];
            break;
        case 'purolator':
            $config = [
                'api_key' => get_post('purolator_api_key', ''),
                'activation_key' => get_post('purolator_activation_key', ''),
                'account_number' => get_post('purolator_account_number', ''),
                'test_mode' => get_post('purolator_test_mode', 0)
            ];
            break;
        case 'canpar':
            $config = [
                'api_key' => get_post('canpar_api_key', ''),
                'username' => get_post('canpar_username', ''),
                'password' => get_post('canpar_password', ''),
                'guest_quotes' => get_post('canpar_guest_quotes', 1)
            ];
            break;
    }
    
    update_company_option("shipping_{$carrier}_config", json_encode($config));
    display_notification(_('Configuration saved successfully'));
}

/**
 * Get carrier configuration
 */
function get_carrier_config($carrier) {
    $config_json = get_company_option("shipping_{$carrier}_config", '{}');
    return json_decode($config_json, true) ?? [];
}

/**
 * Validate carrier configuration
 */
function validate_carrier_config($carrier) {
    $config = get_carrier_config($carrier);
    $valid = false;
    
    switch($carrier) {
        case 'canada_post':
            $valid = !empty($config['api_key']) && !empty($config['customer_number']);
            break;
        case 'ups':
            $valid = !empty($config['oauth_token']) || (!empty($config['username']) && !empty($config['password']));
            break;
        case 'fedex':
            $valid = !empty($config['client_id']) && !empty($config['client_secret']);
            break;
        case 'dhl':
            $valid = !empty($config['api_key']);
            break;
        case 'purolator':
            $valid = !empty($config['api_key']) && !empty($config['activation_key']);
            break;
        case 'canpar':
            $valid = !empty($config['api_key']) || (!empty($config['username']) && !empty($config['password']));
            break;
    }
    
    return $valid;
}
