# Use Cases - FA Shipping Module

## Document Information
- **Module**: ksf_FA_Shipping
- **Type**: FrontAccounting Platform Adapter
- **Version**: 1.0.0
- **Date**: 2026-05-13

---

## 1. Actor Definitions

### 1.1 Primary Actors

| Actor | Description | Permissions |
|-------|-------------|-------------|
| **Admin User** | FA administrator | Full system access |
| **Sales Staff** | Order entry personnel | SA_SALESORDER |
| **Customer** | End customer (indirect) | Via staff actions |

### 1.2 Secondary Actors

| Actor | Description |
|-------|-------------|
| **FA System** | FrontAccounting ERP backend |
| **ksf_Shipping_Core** | Core shipping calculation logic |
| **Carrier APIs** | External shipping carrier APIs |

---

## 2. Use Cases

### UC-SHIP-001: Configure Carrier API Credentials

**Description**: Admin configures shipping carrier API credentials for use in rate calculations.

| Attribute | Value |
|-----------|-------|
| ID | UC-SHIP-001 |
| Actor | Admin User |
| Goal | Set up carrier integration for shipping rates |
| Priority | High |

**Pre-conditions**:
- User has admin access to FA
- Carrier API account obtained from carrier

**Post-conditions**:
- Carrier credentials stored in company options
- Carrier enabled for rate calculation

**Basic Flow - Canada Post Configuration**:
```
1. Admin navigates to Shipping setup (Sales > Setup > Shipping Carriers)
2. Admin clicks "Configure" for Canada Post
3. System displays Canada Post configuration form:
   - API Key (text input)
   - Customer Number (text input)
   - Contract ID (text input, optional)
   - Test Mode (yes/no checkbox)
4. Admin enters API key: "CPC_API_KEY_12345"
5. Admin enters customer number: "123456789"
6. Admin optionally enables test mode
7. Admin clicks "Save Configuration"
8. System validates required fields (API Key, Customer Number)
9. System saves configuration to company options
10. System displays success: "Canada Post configuration saved"
11. Carrier now available for rate calculations
```

**Alternative Flows**:

*AF-SHIP-001a: Missing Required Fields*
```
6a. API Key field empty
6b. System displays: "API Key is required"
6c. Admin enters API key and resubmits
```

*AF-SHIP-001b: Invalid Configuration*
```
8a. API key format invalid
8b. System displays warning (not blocking)
8c. Carrier can still be saved but may fail at runtime
```

---

### UC-SHIP-002: Configure Multiple Carriers

**Description**: Admin configures multiple shipping carriers for comparison.

| Attribute | Value |
|-----------|-------|
| ID | UC-SHIP-002 |
| Actor | Admin User |
| Goal | Enable multiple carrier options for customers |
| Priority | High |

**Pre-conditions**: Admin has access to carrier credentials

**Post-conditions**: Multiple carriers configured and enabled

**Basic Flow**:
```
1. Admin completes UC-SHIP-001 for Canada Post
2. Admin clicks "Configure" for UPS
3. Admin enters UPS credentials:
   - OAuth Token
   - Shipper Number
   - Access License Number
4. Admin saves UPS configuration
5. Admin repeats for FedEx, DHL, Purolator, Canpar
6. System enables all configured carriers
7. All enabled carriers participate in rate calculation
```

---

### UC-SHIP-003: Calculate Shipping Rates for Order

**Description**: Staff calculates shipping rates during order entry.

| Attribute | Value |
|-----------|-------|
| ID | UC-SHIP-003 |
| Actor | Sales Staff |
| Goal | Get shipping rate options for customer order |
| Priority | High |

**Pre-conditions**:
- Sales order exists or being created
- Customer address entered
- At least one carrier configured
- ksf_Shipping_Core available

**Post-conditions**:
- Shipping quotes displayed to user
- User can select shipping method

**Basic Flow**:
```
1. Staff creates or edits sales order
2. Staff enters customer shipping address:
   - Company: "ABC Corporation"
   - Address: "123 Main Street"
   - City: "Toronto"
   - Province: "ON"
   - Postal Code: "M5V 1A1"
   - Country: "CA"
3. Staff adds order line items:
   - Product A (qty: 2, weight: 1.5kg each)
   - Product B (qty: 1, weight: 3kg each)
4. Staff clicks "Calculate Shipping" or equivalent button
5. System retrieves store address from FA company prefs
6. System calculates total package weight: 2×1.5 + 1×3 = 6kg
7. System initializes shipping calculator
8. System registers all configured carriers
9. System queries each carrier API for rates
10. System aggregates quotes from all carriers
11. System displays shipping quote selection interface:
    ┌──────────┬────────────┬───────┬──────────┬────────┐
    │ Carrier  │ Service    │ Rate  │ Transit  │ Select │
    ├──────────┼────────────┼───────┼──────────┼────────┤
    │ Canada   │ Express    │ $15.50│ 1 day    │ [Select]│
    │ Post     │ Priority   │ $12.75│ 2 days   │ [Select]│
    │ UPS      │ Ground     │ $11.25│ 3 days   │ [Select]│
    │ FedEx    │ Express    │ $14.00│ 1 day    │ [Select]│
    └──────────┴────────────┴───────┴──────────┴────────┘
12. Staff reviews options and selects preferred method
```

**Alternative Flows**:

*AF-SHIP-003a: No Carriers Configured*
```
7a. No carrier configurations exist
7b. System displays: "No shipping carriers configured. Please configure carriers in Setup."
7c. Staff notifies admin to configure carriers
```

*AF-SHIP-003b: Missing Customer Address*
```
4a. Customer address not entered
4b. System displays: "Customer address required for shipping calculation"
4c. Staff enters address and retries
```

*AF-SHIP-003c: API Timeout*
```
8a. Carrier API does not respond within timeout
8b. System times out and continues with other carriers
8c. System displays: "Some carriers unavailable. Showing X of Y quotes."
```

---

### UC-SHIP-004: Select Shipping Method

**Description**: User selects preferred shipping method from available quotes.

| Attribute | Value |
|-----------|-------|
| ID | UC-SHIP-004 |
| Actor | Sales Staff |
| Goal | Choose shipping method for order |
| Priority | High |

**Pre-conditions**: Shipping quotes displayed (UC-SHIP-003)

**Post-conditions**:
- Shipping method saved to order
- Shipping cost added to order total

**Basic Flow**:
```
1. Staff views shipping quote table (from UC-SHIP-003)
2. Staff evaluates options:
   - Price
   - Transit time
   - Carrier preference
3. Staff selects UPS Ground option:
   - Carrier: UPS
   - Service: Ground
   - Rate: $11.25
4. Staff clicks "Select" button for UPS Ground
5. System saves shipping to order:
   - shipping_carrier = 'ups'
   - shipping_service = 'ground'
   - shipping_cost = 11.25
6. System updates order total:
   - Subtotal: $150.00
   - Shipping: $11.25
   - Total: $161.25
7. System displays updated order summary
```

---

### UC-SHIP-005: View Shipping on Order

**Description**: User views shipping information on an existing order.

| Attribute | Value |
|-----------|-------|
| ID | UC-SHIP-005 |
| Actor | Sales Staff, Admin User |
| Goal | Review shipping details for order |
| Priority | Medium |

**Pre-conditions**: Order has shipping method selected

**Post-conditions**: Shipping details displayed

**Basic Flow**:
```
1. User opens existing sales order
2. System loads order from database
3. System displays order details including:
   - Shipping Carrier: UPS
   - Shipping Service: Ground
   - Shipping Cost: $11.25
   - Tracking Number: (if available, future)
4. Order total shows shipping line item
```

---

### UC-SHIP-006: Change Shipping Method

**Description**: User changes shipping method on an existing order.

| Attribute | Value |
|-----------|-------|
| ID | UC-SHIP-006 |
| Actor | Sales Staff, Admin User |
| Goal | Modify shipping selection |
| Priority | Medium |

**Pre-conditions**: Order exists with initial shipping selection

**Post-conditions**: New shipping method saved

**Basic Flow**:
```
1. User opens sales order
2. User clicks "Change Shipping" or equivalent
3. System displays shipping quote selection
4. User selects different carrier/method
5. System updates shipping on order
6. System recalculates order total
7. New shipping details displayed
```

---

### UC-SHIP-007: Remove Shipping from Order

**Description**: User removes shipping from an order (e.g., customer pickup).

| Attribute | Value |
|-----------|-------|
| ID | UC-SHIP-007 |
| Actor | Admin User |
| Goal | Remove shipping from order |
| Priority | Low |

**Pre-conditions**: Order has shipping method selected

**Post-conditions**: Shipping removed, order total updated

**Basic Flow**:
```
1. Admin opens sales order
2. Admin clicks "Remove Shipping" or sets carrier to "None"
3. System clears shipping fields:
   - shipping_carrier = NULL
   - shipping_service = NULL
   - shipping_cost = 0
4. System recalculates order total (shipping removed)
5. Updated order displayed
```

---

## 3. Use Case Diagram

```
                    ┌─────────────────────┐
                    │   Admin User       │
                    │ (SA_SHIPPING)      │
                    └─────────────────────┘
                                │
           ┌────────────────────┼────────────────────┐
           │                    │                    │
           ▼                    ▼                    ▼
    ┌─────────────┐     ┌─────────────┐     ┌─────────────┐
    │  Configure  │     │  Configure  │     │  Configure  │
    │  Carrier 1  │     │  Carrier 2  │     │  Carrier N  │
    │ (UC-SHIP-001)│     │ (UC-SHIP-001)│     │ (UC-SHIP-001)│
    └─────────────┘     └─────────────┘     └─────────────┘
                                │
                                ▼
                    ┌─────────────────────┐
                    │Configure Multiple   │
                    │   Carriers          │
                    │ (UC-SHIP-002)       │
                    └─────────────────────┘

                    ┌─────────────────────┐
                    │   Sales Staff       │
                    │ (SA_SALESORDER)    │
                    └─────────────────────┘
                                │
           ┌────────────────────┼────────────────────┐
           │                    │                    │
           ▼                    ▼                    ▼
    ┌─────────────┐     ┌─────────────┐     ┌─────────────┐
    │  Calculate  │     │   Select    │     │   View      │
    │   Rates     │────▶│  Shipping   │────▶│  Shipping   │
    │ (UC-SHIP-003)│     │ (UC-SHIP-004)│     │ (UC-SHIP-005)│
    └─────────────┘     └─────────────┘     └─────────────┘
                                │                    │
                                ▼                    ▼
                         ┌─────────────┐     ┌─────────────┐
                         │   Change    │     │  Remove     │
                         │  Shipping   │     │  Shipping   │
                         │ (UC-SHIP-006)│    │ (UC-SHIP-007)│
                         └─────────────┘     └─────────────┘
```

---

## 4. Business Rules

| Rule ID | Description |
|---------|-------------|
| BR-SHIP-001 | Carrier configuration required before rate calculation |
| BR-SHIP-002 | Customer address required for shipping calculation |
| BR-SHIP-003 | Shipping cost added to order total |
| BR-SHIP-004 | Only validated carriers participate in calculation |
| BR-SHIP-005 | Test mode allows sandbox API calls without charges |
| BR-SHIP-006 | Multiple carriers can be configured simultaneously |
| BR-SHIP-007 | Shipping method stored with order for fulfillment |

---

## 5. Requirements to Use Case Mapping

| Requirement | Use Case(s) |
|-------------|-------------|
| FR-SHIP-001 | UC-SHIP-001 |
| FR-SHIP-002 | UC-SHIP-001 |
| FR-SHIP-003 | UC-SHIP-001 |
| FR-SHIP-007 | UC-SHIP-001 |
| FR-SHIP-008 | UC-SHIP-001 |
| FR-SHIP-009 | UC-SHIP-001, UC-SHIP-002 |
| FR-SHIP-010 | UC-SHIP-003 |
| FR-SHIP-011 | UC-SHIP-003 |
| FR-SHIP-012 | UC-SHIP-003 |
| FR-SHIP-013 | UC-SHIP-003 |
| FR-SHIP-014 | UC-SHIP-003 |
| FR-SHIP-020 | UC-SHIP-003 |
| FR-SHIP-021 | UC-SHIP-003 |
| FR-SHIP-030 | UC-SHIP-004 |
| FR-SHIP-031 | UC-SHIP-004 |
| FR-SHIP-040 | UC-SHIP-003 |

---

## 6. Integration with ksf_Shipping_Core

### 6.1 Core Functions Used

| Function | Purpose |
|----------|---------|
| `CanadianShippingCalculator` | Main calculator class |
| `CarrierAdapters` | Carrier adapter classes |
| `getQuotes()` | Calculate rates |
| `setStoreAddress()` | Configure origin |
| `registerCarrier()` | Add carrier to calculator |

### 6.2 Data Transformation

**FA Order → Calculator Input**:
```
FA Order:
  - customer_id → Customer Address Object
  - order_items[] → Package Weight/Dimensions

Calculator Input:
  - customerAddress: {address, city, state, postcode, country}
  - parcel: {weight: float, dimensions...}
```

**Calculator Output → FA Order**:
```
Calculator Output:
  - quotes: [{carrier, service_name, rate, transit_days}...]

FA Order Update:
  - shipping_carrier = selected.carrier
  - shipping_service = selected.service_code
  - shipping_cost = selected.rate
```

---

*Document Version: 1.0.0*
*Last Updated: 2026-05-13*
*Author: KSFII Development Team*