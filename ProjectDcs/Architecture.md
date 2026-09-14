# Architecture - FA Shipping Module

## Document Information
- **Module**: ksf_FA_Shipping
- **Type**: FrontAccounting Platform Adapter
- **Version**: 1.0.0
- **Date**: 2026-05-13

---

## 1. Technical Architecture

### 1.1 Architecture Pattern
The FA Shipping module follows the **Business Logic + Platform Adapter** pattern:

```
┌─────────────────────────────────────────────────────────┐
│                   FrontAccounting UI                     │
├─────────────────────────────────────────────────────────┤
│     carrier_config_ui.inc (Configuration UI)            │
│     shipping_calculator.inc (Rate Calculator UI)        │
├─────────────────────────────────────────────────────────┤
│            shipping_integration.php (Integration)        │
├─────────────────────────────────────────────────────────┤
│                 ksf_Shipping_Core (Core)                │
│    ├── CarrierAdapters.php (Carrier Interface)          │
│    ├── CanadianShippingCalculator.php (Calculator)     │
│    └── Carrier-specific adapters                        │
└─────────────────────────────────────────────────────────┘
```

### 1.2 Module Components

| Component | File | Responsibility |
|-----------|------|----------------|
| Integration | includes/shipping_integration.php | Initial integration file |
| Calculator | includes/shipping_calculator.inc | Rate calculation and display |
| Config UI | includes/carrier_config_ui.inc | Admin configuration interface |
| Hooks | hooks.php (future) | FA integration |

---

## 2. Class Diagram

### 2.1 Shipping Calculator Flow

```
┌─────────────────────────────────────────────────────────────┐
│                  FrontAccounting Order Entry                │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│             shipping_calculator.inc                        │
│  - init_shipping_calculator()                              │
│  - calculate_order_shipping($order_id)                     │
│  - display_shipping_quote_selection($order_id)             │
│  - save_order_shipping($order_id, $service_code, ...)     │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                 Ksf\Shipping\* Adapters                    │
│  - CanadaPostAdapter                                       │
│  - UPS_CanadaAdapter                                       │
│  - FedEx_CanadaAdapter                                     │
│  - DHL_CanadaAdapter                                       │
│  - PurolatorAdapter                                        │
│  - CanparAdapter                                           │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│              CanadianShippingCalculator                     │
│  - getQuotes($customerAddress, $parcel)                   │
│  - setStoreAddress($address)                               │
│  - registerCarrier($adapter)                               │
└─────────────────────────────────────────────────────────────┘
```

### 2.2 Carrier Adapter Interface

```
┌─────────────────────────────────────────────────────────────┐
│                    CarrierAdapter (Interface)               │
├─────────────────────────────────────────────────────────────┤
│ + getName(): string                                         │
│ + calculateRate($shipment): ShippingRate                   │
│ + getServices(): Service[]                                │
│ + validateConfig($config): bool                            │
└─────────────────────────────────────────────────────────────┘
                              │
          ┌───────────────────┼───────────────────┐
          │                   │                   │
          ▼                   ▼                   ▼
┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐
│  CanadaPost     │ │   UPS_Canada    │ │    FedEx_CA     │
│    Adapter      │ │    Adapter      │ │    Adapter      │
└─────────────────┘ └─────────────────┘ └─────────────────┘
          │                   │                   │
          │                   │                   │
          ▼                   ▼                   ▼
┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐
│ Canada Post API │ │   UPS OAuth    │ │   FedEx OAuth   │
└─────────────────┘ └─────────────────┘ └─────────────────┘
```

---

## 3. Data Flow

### 3.1 Shipping Rate Calculation Flow

```
┌─────────────┐      ┌─────────────┐      ┌─────────────┐
│   Order     │      │  shipping   │      │    ksf_     │
│   Entry     │─────▶│_calculator  │─────▶│ Shipping    │
│   Page      │      │   .inc      │      │  _Core      │
└─────────────┘      └─────────────┘      └─────────────┘
                            │                    │
                            │                    ▼
                     ┌─────────────┐      ┌─────────────┐
                     │ Get Carrier │      │  Carrier    │
                     │   Config    │      │  Adapters   │
                     └─────────────┘      └─────────────┘
                            │                    │
                            ▼                    ▼
                     ┌─────────────┐      ┌─────────────┐
                     │ FA Company  │      │  External   │
                     │  Options    │      │  Carrier    │
                     └─────────────┘      │   APIs      │
                                          └─────────────┘
```

### 3.2 Order Shipping Save Flow

```
┌─────────────┐      ┌─────────────┐      ┌─────────────┐
│   User     │      │  shipping   │      │   FA Sales  │
│  Selects   │─────▶│_calculator  │─────▶│   Orders    │
│   Rate     │      │   .inc      │      │   Table     │
└─────────────┘      └─────────────┘      └─────────────┘
                            │
                            ▼
                     ┌─────────────┐
                     │   Update:   │
                     │ - carrier   │
                     │ - service   │
                     │ - cost      │
                     └─────────────┘
```

---

## 4. Configuration Architecture

### 4.1 Carrier Configuration Storage

Configuration stored in FA company options table:
```
0_company_options
├── id
├── category
├── label
└── value (JSON encoded)
```

Each carrier stored with key pattern:
```
shipping_{carrier}_config
```

### 4.2 Configuration Schema by Carrier

**Canada Post**:
```json
{
    "api_key": "string",
    "customer_number": "string",
    "contract_id": "string (optional)",
    "test_mode": 0|1
}
```

**UPS**:
```json
{
    "oauth_token": "string",
    "shipper_number": "string",
    "access_license": "string",
    "test_mode": 0|1
}
```

**FedEx**:
```json
{
    "client_id": "string",
    "client_secret": "string",
    "account_number": "string",
    "test_mode": 0|1
}
```

**DHL**:
```json
{
    "api_key": "string",
    "account_number": "string",
    "test_mode": 0|1
}
```

**Purolator**:
```json
{
    "api_key": "string",
    "activation_key": "string",
    "account_number": "string",
    "test_mode": 0|1
}
```

**Canpar**:
```json
{
    "api_key": "string",
    "username": "string",
    "password": "string",
    "guest_quotes": 0|1
}
```

---

## 5. Integration Points

### 5.1 FrontAccounting Integration

| Integration Point | Purpose | Implementation |
|-------------------|---------|----------------|
| Company Options | Carrier config storage | get_company_option(), update_company_option() |
| Sales Orders | Shipping on orders | Order update in save_order_shipping() |
| Session | Store address | get_company_prefs() |

### 5.2 ksf_Shipping_Core Integration

| Function | Usage |
|----------|-------|
| CarrierAdapters.php | Adapter classes |
| CanadianShippingCalculator.php | Rate calculation engine |
| create_carrier_adapter() | FA function to instantiate adapters |

---

## 6. Menu Structure (Planned)

### 6.1 Sales Application Menu

```
Sales Application
├── Setup (future)
│   └── Shipping Carriers      [SA_SHIPPING]
├── Orders (future)
│   └── Shipping Calculator    [SA_SALESORDER]
```

---

## 7. File Structure

```
ksf_FA_Shipping/
├── hooks.php                   # FA hooks (planned)
├── includes/
│   ├── shipping_integration.php # Initial integration
│   ├── shipping_calculator.inc  # Rate calculation UI
│   └── carrier_config_ui.inc   # Admin configuration
├── sql/                         # Schema files (future)
├── docs/
│   └── Requirements.md          # Requirements doc
└── ProjectDcs/
    ├── Business_Requirements.md
    ├── Architecture.md          # This file
    ├── Functional_Requirements.md
    ├── Use_Case.md
    ├── Test_Plan.md
    └── UAT_Plan.md
```

---

## 8. Security Architecture

### 8.1 Credential Handling
- API credentials stored in database (company options)
- Display fields use password type for secrets
- Test mode available to prevent live charges
- Credentials validated before use

### 8.2 Data Validation
- validate_carrier_config() checks required fields
- get_carrier_config() returns JSON decoded array
- db_escape() not needed (using FA get_post() wrapper)

---

## 9. Error Handling

### 9.1 Carrier API Errors
- Missing config: validate_carrier_config() returns false
- Invalid credentials: API returns error, displayed to user
- Rate calculation failure: Empty array returned

### 9.2 Order Integration Errors
- Order not found: calculate_order_shipping() returns empty
- Missing customer address: Returns empty quotes
- Missing product weights: Calculates with 0 weight

---

## 10. Performance Considerations

### 10.1 Rate Calculation
- Multiple carrier API calls can be slow
- Future: Add rate caching
- Configuration stored in company options (cached by FA)

### 10.2 Quote Display
- All carrier quotes shown in single table
- Sorted by rate (lowest first) or carrier
- Transit time included for comparison

---

## 11. Future Enhancements

### 11.1 Planned Hooks
```php
// hooks.php (planned)
class hooks_fa_shipping extends hooks {
    function install_options($app) {
        // Add Shipping menu items
    }
    
    function install_access() {
        // Add SA_SHIPPING security area
    }
    
    function activate_extension($company, $check_only) {
        // Create shipping tables if needed
    }
}
```

### 11.2 Planned Database Tables
```sql
-- Shipping methods table (future)
CREATE TABLE fa_shipping_methods (
    id INT PRIMARY KEY,
    carrier VARCHAR(50),
    service_code VARCHAR(50),
    service_name VARCHAR(100),
    active BOOLEAN DEFAULT 1
);

-- Shipping zones table (future)
CREATE TABLE fa_shipping_zones (
    id INT PRIMARY KEY,
    name VARCHAR(100),
    country VARCHAR(100),
    state VARCHAR(100),
    postcode_pattern VARCHAR(50)
);
```

---

*Document Version: 1.0.0*
*Last Updated: 2026-05-13*
*Author: KSFII Development Team*