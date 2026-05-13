# Functional Requirements - FA Shipping Module

## Document Information
- **Module**: ksf_FA_Shipping
- **Type**: FrontAccounting Platform Adapter
- **Version**: 1.0.0
- **Date**: 2026-05-13

---

## 1. Introduction

### 1.1 Purpose
This document defines the functional requirements for the FA Shipping module, a FrontAccounting platform adapter that provides shipping rate calculation, carrier configuration, and order integration.

### 1.2 Scope
These requirements cover carrier configuration management, shipping rate calculation, quote display, and order shipping method association.

---

## 2. Functional Requirements

### 2.1 Carrier Configuration

#### FR-SHIP-001: Configure Canada Post
- **Description**: Admin can configure Canada Post API credentials
- **Preconditions**: User has admin access
- **Fields**:
  | Field | Required | Description |
  |-------|----------|-------------|
  | API Key | Yes | Canada Post API key |
  | Customer Number | Yes | Canada Post customer number |
  | Contract ID | No | Contract rate ID (optional) |
  | Test Mode | No | Enable test/sandbox mode |
- **Validation**: API Key and Customer Number required
- **Priority**: High

#### FR-SHIP-002: Configure UPS
- **Description**: Admin can configure UPS Canada API credentials
- **Fields**:
  | Field | Required | Description |
  |-------|----------|-------------|
  | OAuth Token | No* | UPS OAuth token |
  | Shipper Number | No* | UPS shipper number |
  | Access License | No* | UPS access license |
  | Test Mode | No | Enable test mode |
- **Validation**: OAuth Token OR (Shipper + Access License) required
- **Priority**: High

#### FR-SHIP-003: Configure FedEx
- **Description**: Admin can configure FedEx Canada API credentials
- **Fields**:
  | Field | Required | Description |
  |-------|----------|-------------|
  | Client ID | Yes | FedEx OAuth client ID |
  | Client Secret | Yes | FedEx OAuth client secret |
  | Account Number | No | FedEx account number |
  | Test Mode | No | Enable test mode |
- **Priority**: Medium

#### FR-SHIP-004: Configure DHL
- **Description**: Admin can configure DHL Canada API credentials
- **Fields**:
  | Field | Required | Description |
  |-------|----------|-------------|
  | API Key | Yes | DHL API key |
  | Account Number | No | DHL account number |
  | Test Mode | No | Enable test mode |
- **Priority**: Medium

#### FR-SHIP-005: Configure Purolator
- **Description**: Admin can configure Purolator API credentials
- **Fields**:
  | Field | Required | Description |
  |-------|----------|-------------|
  | API Key | Yes | Purolator API key |
  | Activation Key | Yes | Purolator activation key |
  | Account Number | No | Purolator account number |
  | Test Mode | No | Enable test mode |
- **Priority**: Medium

#### FR-SHIP-006: Configure Canpar
- **Description**: Admin can configure Canpar API credentials
- **Fields**:
  | Field | Required | Description |
  |-------|----------|-------------|
  | API Key | No* | Canpar API key |
  | Username | No* | Canpar username |
  | Password | No* | Canpar password |
  | Guest Quotes | No | Allow guest rate quotes |
- **Validation**: API Key OR (Username + Password) required
- **Priority**: Low

#### FR-SHIP-007: Save Carrier Configuration
- **Description**: System saves carrier configuration to company options
- **Implementation**: update_company_option("shipping_{carrier}_config", JSON)
- **Expected Results**: Configuration persisted, success notification shown
- **Priority**: High

#### FR-SHIP-008: Validate Carrier Configuration
- **Description**: System validates carrier config before saving
- **Implementation**: validate_carrier_config($carrier)
- **Expected Results**: Returns true if valid, false otherwise
- **Priority**: High

#### FR-SHIP-009: Retrieve Carrier Configuration
- **Description**: System retrieves stored carrier configuration
- **Implementation**: get_carrier_config($carrier)
- **Expected Results**: Returns config array or empty array
- **Priority**: High

---

### 2.2 Shipping Rate Calculation

#### FR-SHIP-010: Calculate Shipping Rates for Order
- **Description**: System calculates shipping rates for a sales order
- **Preconditions**: Valid order ID, customer address present
- **Trigger**: User initiates shipping calculation during order entry
- **Inputs**:
  - Order ID (FA sales order)
  - Store address (from FA company prefs)
  - Customer address (from order/customer)
  - Package dimensions/weight (from order items)
- **Output**: Array of shipping quotes from all configured carriers
- **Priority**: High

#### FR-SHIP-011: Get Customer Address
- **Description**: System retrieves customer shipping address from order
- **Implementation**: Extract from sales order and customer records
- **Fields Retrieved**:
  - Company name
  - Street address
  - City
  - State/Province
  - Postal code
  - Country
- **Priority**: High

#### FR-SHIP-012: Calculate Package Weight
- **Description**: System calculates total package weight from order items
- **Implementation**: Sum of (item quantity × item weight)
- **Note**: Requires weight field in stock_master table
- **Priority**: High

#### FR-SHIP-013: Initialize Shipping Calculator
- **Description**: Initialize calculator with store address and registered carriers
- **Implementation**: init_shipping_calculator()
- **Expected Results**: Calculator ready to calculate rates
- **Priority**: High

#### FR-SHIP-014: Create Carrier Adapters
- **Description**: Create appropriate carrier adapter instances
- **Implementation**: create_carrier_adapter($carrier, $config)
- **Supported Carriers**: canada_post, ups, fedex, dhl, purolator, canpar
- **Priority**: High

---

### 2.3 Quote Display

#### FR-SHIP-020: Display Shipping Quote Selection
- **Description**: Display available shipping options to user
- **Implementation**: display_shipping_quote_selection($order_id)
- **Displayed Columns**:
  | Column | Description |
  |--------|-------------|
  | Carrier | Shipping carrier name |
  | Service | Service level/method |
  | Rate | Cost in currency |
  | Transit Days | Estimated delivery time |
  | Action | Select button |
- **Priority**: High

#### FR-SHIP-021: Handle No Quotes Available
- **Description**: Handle case when no shipping quotes returned
- **Implementation**: Check for empty quotes array
- **User Message**: "No shipping quotes available. Please check carrier configuration."
- **Priority**: Medium

---

### 2.4 Order Integration

#### FR-SHIP-030: Save Selected Shipping to Order
- **Description**: Save selected shipping method to sales order
- **Implementation**: save_order_shipping($order_id, $service_code, $rate, $carrier)
- **Database Update**:
  ```sql
  UPDATE sales_orders SET
      shipping_carrier = '$carrier',
      shipping_service = '$service_code',
      shipping_cost = $rate
  WHERE order_no = $order_id
  ```
- **Priority**: High

#### FR-SHIP-031: Include Shipping in Order Total
- **Description**: Shipping cost included in order total calculation
- **Implementation**: FA order total calculation includes shipping_cost
- **Priority**: High

---

### 2.5 Store Configuration

#### FR-SHIP-040: Get Store Address from FA
- **Description**: Retrieve store/sender address from FA company preferences
- **Implementation**: get_company_prefs()
- **Fields**:
  - Company name
  - Postal address
  - City
  - State
  - Postal code
  - Country
- **Priority**: High

---

## 3. Non-Functional Requirements

### 3.1 Performance
- Rate calculation (single order, single carrier): < 200ms
- Rate calculation (single order, 6 carriers): < 1 second
- Quote display: < 500ms after calculation
- Carrier config save: < 50ms

### 3.2 Security
- API credentials stored securely in database
- Password fields masked in UI
- No credentials logged or displayed in plain text
- Test mode available for sandbox testing

### 3.3 Compatibility
- FrontAccounting 2.4.x compatible
- PHP 7.3+ compatible
- FA coding standards compliance

### 3.4 Integration
- ksf_Shipping_Core dependency
- FA company options storage
- FA sales orders integration

---

## 4. Requirements Traceability

| Requirement ID | Description | Test Case | Status |
|----------------|-------------|-----------|--------|
| FR-SHIP-001 | Configure Canada Post | TC-SHIP-001 | ✓ |
| FR-SHIP-002 | Configure UPS | TC-SHIP-002 | ✓ |
| FR-SHIP-003 | Configure FedEx | TC-SHIP-003 | ✓ |
| FR-SHIP-007 | Save Config | TC-SHIP-007 | ✓ |
| FR-SHIP-008 | Validate Config | TC-SHIP-008 | ✓ |
| FR-SHIP-010 | Calculate Rates | TC-SHIP-010 | ✓ |
| FR-SHIP-020 | Display Quotes | TC-SHIP-020 | ✓ |
| FR-SHIP-030 | Save to Order | TC-SHIP-030 | ✓ |

---

*Document Version: 1.0.0*
*Last Updated: 2026-05-13*
*Author: KSFII Development Team*