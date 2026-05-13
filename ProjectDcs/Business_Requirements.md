# Business Requirements - FA Shipping Module

## Document Information
- **Module**: ksf_FA_Shipping
- **Type**: FrontAccounting Platform Adapter
- **Version**: 1.0.0
- **Date**: 2026-05-13
- **Status**: Implemented

---

## 1. Project Overview

### 1.1 Module Purpose
The FA Shipping module is a FrontAccounting platform adapter that integrates shipping carrier functionality with the FA order management system. It enables organizations to calculate shipping rates from multiple carriers, configure carrier credentials, and associate shipping costs with sales orders.

### 1.2 Problem Statement
Organizations require mechanisms to:
- Calculate real-time shipping rates during order entry
- Support multiple shipping carriers (Canada Post, UPS, FedEx, DHL, Purolator, Canpar)
- Configure carrier API credentials securely
- Display shipping options to customers
- Save selected shipping method to orders
- Integrate shipping costs into order totals

This module addresses these needs by providing shipping calculation functionality within FrontAccounting.

### 1.3 Module Type
**Thin Platform Adapter**: This module consumes business logic from `ksf_Shipping_Core` and provides FA-specific UI, configuration, and order integration.

---

## 2. Scope

### 2.1 In Scope
- Shipping carrier configuration management
- Carrier API credential storage
- Shipping rate calculation for orders
- Quote display and selection interface
- Order shipping method storage
- Store address configuration from FA company settings
- Multiple carrier support (6 Canadian carriers)

### 2.2 Out of Scope
- Label generation
- Shipment tracking
- Return shipping processing
- Multi-currency pricing
- Warehouse/location-based shipping
- Rate caching (future enhancement)
- Shipping rule engine (conditional rates)

### 2.3 Dependencies
| Dependency | Type | Purpose |
|------------|------|---------|
| FrontAccounting 2.4.x | Platform | Target ERP system |
| ksf_Shipping_Core | Business Logic | Core shipping calculator |
| Sales Orders | FA Module | Order integration |

---

## 3. Features

### 3.1 Core Features

#### F1: Carrier Configuration
- Configure multiple shipping carriers
- Store API keys and credentials securely
- Enable/disable carriers individually
- Support for 6 carriers:
  - Canada Post
  - UPS (Canada)
  - FedEx (Canada)
  - DHL (Canada)
  - Purolator
  - Canpar

#### F2: Shipping Rate Calculation
- Calculate rates during order entry
- Integration with ksf_Shipping_Core calculator
- Support multiple packages/parcels
- Weight and dimension handling
- Address validation

#### F3: Quote Display
- Display available shipping options
- Show carrier, service, rate, transit time
- Allow customer selection
- Save selection to order

#### F4: Order Integration
- Link shipping method to sales order
- Store carrier and service code
- Store shipping cost
- Include in order total calculation

### 3.2 Supported Carriers

| Carrier | API Type | Key Credentials |
|---------|----------|-----------------|
| Canada Post | API Key + Customer Number | api_key, customer_number, contract_id |
| UPS | OAuth + Account | oauth_token, shipper_number, access_license |
| FedEx | OAuth | client_id, client_secret, account_number |
| DHL | API Key | api_key, account_number |
| Purolator | API Key + Activation | api_key, activation_key, account_number |
| Canpar | API Key + Credentials | api_key, username, password |

---

## 4. Functional Overview

### 4.1 Configuration Storage

Configuration stored in FA company options:
```
shipping_{carrier}_config = JSON {
    "api_key": "...",
    "customer_number": "...",
    "test_mode": 0/1,
    ...
}
```

### 4.2 Menu Integration

| Location | Function | Security | Purpose |
|----------|----------|----------|---------|
| Sales > Setup | Shipping Carriers | SA_SHIPPING | Configure carriers |
| Sales > Orders | Shipping Calculator | SA_SALESORDER | Rate calculation |

---

## 5. Integration Architecture

### 5.1 Module Architecture
```
ksf_Shipping_Core (Business Logic)
    │
    ├── CarrierAdapters.php
    ├── CanadianShippingCalculator.php
    │
    ↓
ksf_FA_Shipping (FA Adapter)
    ├── hooks.php (FA Integration)
    ├── includes/shipping_calculator.inc (Calculator UI)
    ├── includes/carrier_config_ui.inc (Configuration)
    └── includes/shipping_integration.php (Order Integration)
```

### 5.2 FrontAccounting Integration Points

| Hook | Purpose |
|------|---------|
| install_options() | Register menu items (future) |
| install_access() | Define security areas (future) |
| activate_extension() | Module activation (future) |

---

## 6. Security Considerations

### 6.1 Credential Storage
- API credentials stored in company options table
- Credentials not displayed in plain text (password field type)
- Test mode available for sandbox testing

### 6.2 Data Validation
- Carrier configuration validated before save
- Required fields checked per carrier
- Test mode toggle available

---

## 7. Performance Requirements

| Metric | Target |
|--------|--------|
| Rate calculation (single order) | < 200ms for 10 packages |
| Quote display | < 1 second total |
| Carrier config save | < 50ms |

---

## 8. Future Enhancements

| Feature | Priority | Description |
|---------|----------|-------------|
| Label generation | High | Create shipping labels |
| Tracking integration | Medium | Track shipments |
| Rate caching | Medium | Cache rates to reduce API calls |
| Shipping rules | Low | Conditional shipping methods |
| Multi-warehouse | Low | Location-based shipping |

---

*Document Version: 1.0.0*
*Last Updated: 2026-05-13*
*Author: KSFII Development Team*