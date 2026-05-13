# Test Plan - FA Shipping Module

## Document Information
- **Module**: ksf_FA_Shipping
- **Type**: FrontAccounting Platform Adapter
- **Version**: 1.0.0
- **Date**: 2026-05-13

---

## 1. Introduction

### 1.1 Purpose
This Test Plan defines the testing approach, test scenarios, test data requirements, and pass criteria for the FA Shipping module.

### 1.2 Scope
Testing covers carrier configuration management, shipping rate calculation, quote display, and order integration with ksf_Shipping_Core.

### 1.3 Test Environment
- **Platform**: FrontAccounting 2.4.x
- **Database**: MySQL 5.7+
- **PHP Version**: 7.3+
- **Browser**: Chrome/Firefox/Safari (latest)
- **Dependencies**: ksf_Shipping_Core, FA Sales module

---

## 2. Test Strategy

### 2.1 Test Types

| Test Type | Description | Coverage |
|-----------|-------------|----------|
| Unit Testing | Individual function testing | includes/*.inc functions |
| Integration Testing | ksf_Shipping_Core integration | Calculator functions |
| UI Testing | Configuration UI testing | carrier_config_ui.inc |
| API Mock Testing | Carrier API simulation | Simulated carrier responses |

---

## 3. Test Scenarios

### 3.1 Carrier Configuration Tests

#### TC-SHIP-001: Configure Canada Post
| Attribute | Value |
|-----------|-------|
| Test ID | TC-SHIP-001 |
| Requirement | FR-SHIP-001 |
| Priority | High |

**Test Steps**:
```
1. Call display_carrier_config_form('canada_post')
2. Verify form fields: api_key, customer_number, contract_id, test_mode
3. Submit valid configuration
4. Call save_carrier_config('canada_post')
5. Verify config saved to company options
6. Call get_carrier_config('canada_post')
7. Verify returned config matches submitted data
```

**Test Data**:
```php
$config = [
    'api_key' => 'CPC_TEST_API_KEY',
    'customer_number' => '123456789',
    'contract_id' => 'CONTRACT001',
    'test_mode' => 1
];
```

**Pass Criteria**: 
- ✓ Form displays all fields
- ✓ Configuration saved correctly
- ✓ Configuration retrieved correctly

---

#### TC-SHIP-002: Configure UPS
| Attribute | Value |
|-----------|-------|
| Test ID | TC-SHIP-002 |
| Requirement | FR-SHIP-002 |
| Priority | High |

**Test Steps**:
```
1. Call display_canada_post_config() - skipped, testing UPS
2. Submit UPS configuration
3. Verify config saved correctly
```

**Test Data**:
```php
$config = [
    'oauth_token' => 'UPS_OAUTH_TOKEN',
    'shipper_number' => 'SHP12345',
    'access_license' => 'UPS_ACCESS_LIC',
    'test_mode' => 1
];
```

**Pass Criteria**: 
- ✓ UPS config fields validated
- ✓ Config saved correctly

---

#### TC-SHIP-003: Configure FedEx
| Attribute | Value |
|-----------|-------|
| Test ID | TC-SHIP-003 |
| Requirement | FR-SHIP-003 |
| Priority | Medium |

**Test Steps**:
```
1. Submit FedEx configuration with client_id and client_secret
2. Verify config saved correctly
```

**Test Data**:
```php
$config = [
    'client_id' => 'fedex_client_id',
    'client_secret' => 'fedex_secret',
    'account_number' => 'FEDEX_ACCOUNT',
    'test_mode' => 1
];
```

**Pass Criteria**: 
- ✓ FedEx config saved correctly

---

#### TC-SHIP-007: Save Carrier Configuration
| Attribute | Value |
|-----------|-------|
| Test ID | TC-SHIP-007 |
| Requirement | FR-SHIP-007 |
| Priority | High |

**Test Steps**:
```
1. Call save_carrier_config('canada_post') with valid data
2. Verify update_company_option() called
3. Verify JSON encoding of config
4. Verify success notification displayed
```

**Pass Criteria**: 
- ✓ Config stored in company options
- ✓ JSON format correct
- ✓ Notification shown

---

#### TC-SHIP-008: Validate Carrier Configuration
| Attribute | Value |
|-----------|-------|
| Test ID | TC-SHIP-008 |
| Requirement | FR-SHIP-008 |
| Priority | High |

**Test Steps**:
```
1. Save valid Canada Post config
2. Call validate_carrier_config('canada_post')
3. Verify returns true
4. Save empty Canada Post config
5. Call validate_carrier_config('canada_post')
6. Verify returns false
```

**Pass Criteria**: 
- ✓ Valid config returns true
- ✓ Invalid config returns false
- ✓ Required fields enforced

---

### 3.2 Rate Calculation Tests

#### TC-SHIP-010: Calculate Shipping Rates
| Attribute | Value |
|-----------|-------|
| Test ID | TC-SHIP-010 |
| Requirement | FR-SHIP-010 |
| Priority | High |

**Pre-conditions**: At least one carrier configured with valid credentials (or test mode)

**Test Steps**:
```
1. Create test order with customer address
2. Call calculate_order_shipping($order_id)
3. Verify returns array of quotes
4. Verify each quote has: carrier, service, rate, transit_days
5. Verify quotes sorted appropriately
```

**Test Data**:
```
Order: Order #123
Customer Address:
  - 123 Main St, Toronto, ON, M5V 1A1, Canada

Package:
  - Weight: 5kg
  - Default dimensions

Expected Quotes (example):
  - Canada Post Express: $15.50, 1 day
  - UPS Ground: $11.25, 3 days
  - FedEx Priority: $18.00, 1 day
```

**Pass Criteria**: 
- ✓ Quotes returned
- ✓ Quote structure correct
- ✓ Rate values present
- ✓ Carrier names present

---

#### TC-SHIP-011: Get Customer Address from Order
| Attribute | Value |
|-----------|-------|
| Test ID | TC-SHIP-011 |
| Requirement | FR-SHIP-011 |
| Priority | High |

**Test Steps**:
```
1. Create test order with complete customer address
2. Call calculate_order_shipping()
3. Verify customer address passed to calculator
4. Verify address contains all required fields
```

**Pass Criteria**: 
- ✓ Customer address extracted
- ✓ All address fields present
- ✓ Address passed to carrier APIs

---

#### TC-SHIP-012: Calculate Package Weight
| Attribute | Value |
|-----------|-------|
| Test ID | TC-SHIP-012 |
| Requirement | FR-SHIP-012 |
| Priority | High |

**Test Steps**:
```
1. Create test order with items
2. Each item has weight defined in stock_master
3. Call calculate_order_shipping()
4. Verify total weight calculated: sum(qty × weight)
```

**Test Data**:
```
Order Items:
  - Item A: qty 2, weight 1.5kg each → total 3kg
  - Item B: qty 1, weight 2.0kg → total 2kg
  - Item C: qty 3, weight 0.5kg each → total 1.5kg
  
Total Weight: 6.5kg
```

**Pass Criteria**: 
- ✓ Weight summed correctly
- ✓ Weight passed to calculator

---

#### TC-SHIP-013: Initialize Shipping Calculator
| Attribute | Value |
|-----------|-------|
| Test ID | TC-SHIP-013 |
| Requirement | FR-SHIP-013 |
| Priority | High |

**Test Steps**:
```
1. Configure multiple carriers
2. Call init_shipping_calculator()
3. Verify CanadianShippingCalculator instance returned
4. Verify store address set from FA company prefs
5. Verify carriers registered
```

**Pass Criteria**: 
- ✓ Calculator initialized
- ✓ Store address configured
- ✓ Carriers loaded

---

#### TC-SHIP-014: Create Carrier Adapters
| Attribute | Value |
|-----------|-------|
| Test ID | TC-SHIP-014 |
| Requirement | FR-SHIP-014 |
| Priority | High |

**Test Data**:
```
Carriers to test:
- canada_post
- ups
- fedex
- dhl
- purolator
- canpar
```

**Test Steps**:
```
1. For each carrier, call create_carrier_adapter($carrier, $config)
2. Verify appropriate adapter class returned
3. Verify adapter configured with config
4. Test invalid carrier name - verify null returned
```

**Pass Criteria**: 
- ✓ Each carrier creates appropriate adapter
- ✓ Invalid carrier returns null
- ✓ Adapters configured correctly

---

### 3.3 Quote Display Tests

#### TC-SHIP-020: Display Shipping Quote Selection
| Attribute | Value |
|-----------|-------|
| Test ID | TC-SHIP-020 |
| Requirement | FR-SHIP-020 |
| Priority | High |

**Test Steps**:
```
1. Get quotes for test order
2. Call display_shipping_quote_selection($order_id)
3. Verify HTML table output
4. Verify columns: Carrier, Service, Rate, Transit Days, Action
5. Verify each quote has select button
```

**Pass Criteria**: 
- ✓ Table HTML generated
- ✓ All columns present
- ✓ Select buttons rendered

---

#### TC-SHIP-021: Handle No Quotes Available
| Attribute | Value |
|-----------|-------|
| Test ID | TC-SHIP-021 |
| Requirement | FR-SHIP-021 |
| Priority | Medium |

**Test Steps**:
```
1. Disable all carriers (no config)
2. Call display_shipping_quote_selection($order_id)
3. Verify "No shipping quotes available" message shown
4. Verify no table rendered
```

**Pass Criteria**: 
- ✓ Error message displayed
- ✓ User-friendly message shown
- ✓ No errors thrown

---

### 3.4 Order Integration Tests

#### TC-SHIP-030: Save Shipping to Order
| Attribute | Value |
|-----------|-------|
| Test ID | TC-SHIP-030 |
| Requirement | FR-SHIP-030 |
| Priority | High |

**Test Steps**:
```
1. Create test order
2. Call save_order_shipping($order_id, 'ups_ground', 11.25, 'ups')
3. Verify sales_orders updated:
   - shipping_carrier = 'ups'
   - shipping_service = 'ups_ground'
   - shipping_cost = 11.25
```

**Pass Criteria**: 
- ✓ Order updated with shipping
- ✓ All fields set correctly
- ✓ Order total reflects shipping cost

---

#### TC-SHIP-031: Verify Shipping in Order Total
| Attribute | Value |
|-----------|-------|
| Test ID | TC-SHIP-031 |
| Requirement | FR-SHIP-031 |
| Priority | High |

**Test Steps**:
```
1. Get order total before shipping
2. Save shipping to order
3. Get order total after shipping
4. Verify: new_total = old_total + shipping_cost
```

**Pass Criteria**: 
- ✓ Shipping cost added to total
- ✓ Total calculation correct

---

### 3.5 Store Configuration Tests

#### TC-SHIP-040: Get Store Address from FA
| Attribute | Value |
|-----------|-------|
| Test ID | TC-SHIP-040 |
| Requirement | FR-SHIP-040 |
| Priority | High |

**Test Steps**:
```
1. Call init_shipping_calculator()
2. Verify get_company_prefs() called
3. Verify store address includes:
   - Company name
   - Address
   - City
   - State
   - Postal code
   - Country (defaulted to 'CA')
```

**Pass Criteria**: 
- ✓ Store address extracted
- ✓ All fields present
- ✓ Default country set

---

## 4. Security Tests

### TC-SHIP-SEC-001: Credential Storage Security
| Test ID | TC-SHIP-SEC-001 |
|---------|---------------|
| Priority | High |

**Test Steps**:
```
1. Save carrier config with sensitive values
2. Verify database stores JSON (not plain text for all)
3. Verify UI uses password field type for secrets
4. Verify secrets not logged
```

**Pass Criteria**: 
- ✓ Secrets not exposed in logs
- ✓ UI uses password type fields
- ✓ JSON storage format

---

### TC-SHIP-SEC-002: Test Mode Verification
| Test ID | TC-SHIP-SEC-002 |
|---------|---------------|
| Priority | Medium |

**Test Steps**:
```
1. Save carrier config with test_mode = 1
2. Call calculate_order_shipping()
3. Verify test mode flag passed to carrier adapter
4. Verify no live charges occur
```

**Pass Criteria**: 
- ✓ Test mode flag preserved
- ✓ Test credentials used

---

## 5. Performance Tests

### TC-SHIP-PERF-001: Single Carrier Rate Calculation
| Test ID | TC-SHIP-PERF-001 |
|---------|-----------------|
| Target | < 200ms per carrier |

**Test Steps**:
```
1. Configure single carrier
2. Call calculate_order_shipping()
3. Measure API response time
```

**Pass Criteria**: ✓ Calculation completes < 200ms

---

### TC-SHIP-PERF-002: Multi-Carrier Rate Calculation
| Test ID | TC-SHIP-PERF-002 |
|---------|-----------------|
| Target | < 1 second for 6 carriers |

**Test Steps**:
```
1. Configure all 6 carriers
2. Call calculate_order_shipping()
3. Measure total response time
```

**Pass Criteria**: ✓ Total time < 1 second

---

## 6. Test Data Setup

### 6.1 Required Test Data

| Entity | Data |
|--------|------|
| FA Company | Valid company with address |
| Test Customers | Customers with complete addresses |
| Test Products | Products with weight/dimensions |
| Mock Credentials | Test mode API credentials |

### 6.2 Mock Carrier Responses

Since real carrier APIs may not be available during testing, create mock responses:
```php
function mock_carrier_response($carrier) {
    return [
        'carrier' => $carrier,
        'service_name' => 'Standard',
        'rate' => 10.00 + rand(0, 10),
        'transit_days' => rand(1, 7)
    ];
}
```

---

## 7. Integration with ksf_Shipping_Core

### 7.1 Dependency Verification

| Component | Verification |
|-----------|--------------|
| CarrierAdapters.php | File exists and loads |
| CanadianShippingCalculator.php | Class exists |
| getQuotes() | Method callable |
| setStoreAddress() | Method callable |
| registerCarrier() | Method callable |

### 7.2 Mock Core for Unit Testing

Create mock classes to test FA integration without ksf_Shipping_Core:
```php
class MockCanadianShippingCalculator {
    public function getQuotes($address, $parcel) {
        return [
            ['carrier' => 'Test', 'rate' => 10.00, ...]
        ];
    }
}
```

---

## 8. Pass Criteria Summary

| Category | Criteria | Target |
|----------|----------|--------|
| Configuration | All carriers configurable | 100% |
| Rate Calculation | Rates calculated for valid orders | 100% |
| Quote Display | Quotes displayed correctly | 100% |
| Order Integration | Shipping saved to order | 100% |
| Performance | Response times met | 100% |

---

## 9. Risk Assessment

| Risk | Impact | Probability | Mitigation |
|------|--------|-------------|------------|
| Carrier API unavailable | High | Medium | Timeout handling, error display |
| ksf_Shipping_Core missing | Critical | Low | Dependency check on activation |
| Rate calculation timeout | Medium | Medium | Async calculation, progress indicator |
| Invalid credentials | Medium | Low | Validation before save |

---

*Document Version: 1.0.0*
*Last Updated: 2026-05-13*
*Author: KSFII Development Team*