# UAT Plan - FA Shipping Module

## Document Information
- **Module**: ksf_FA_Shipping
- **Type**: FrontAccounting Platform Adapter
- **Version**: 1.0.0
- **Date**: 2026-05-13

---

## 1. Introduction

### 1.1 Purpose
This User Acceptance Testing (UAT) Plan defines the objectives, scenarios, and success criteria for validating the FA Shipping module in a production-like environment.

### 1.2 Scope
UAT covers carrier configuration management, shipping rate calculation, quote display and selection, and order shipping integration.

### 1.3 Stakeholders
| Role | Responsibility |
|------|----------------|
| Project Manager | UAT oversight and sign-off |
| Business Analyst | Requirements validation |
| Sales Staff | Functional testing |
| IT Administrator | Technical validation |
| QA Lead | Test execution oversight |

---

## 2. UAT Objectives

### 2.1 Primary Objectives
1. **Functional Validation**: Carrier configuration and rate calculation work correctly
2. **Integration Verification**: Shipping integrates properly with FA orders
3. **Multi-Carrier Support**: All 6 carriers can be configured and used
4. **User Experience**: Shipping selection interface is intuitive
5. **Security Compliance**: API credentials stored securely

### 2.2 Success Criteria
- All critical test scenarios pass
- All high-priority test scenarios pass
- Rate calculation completes successfully
- Order integration functions correctly
- User sign-off obtained from all stakeholder groups

---

## 3. Test Scenarios

### 3.1 Scenario Set 1: Carrier Configuration

#### UAT-SHIP-S1-001: Configure Canada Post
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S1-001 |
| Priority | Critical |
| Precondition | Admin access to FA, Canada Post API account |

**Steps**:
```
1. Log into FrontAccounting as admin
2. Navigate to Sales > Setup > Shipping Carriers (or equivalent)
3. Click "Configure" for Canada Post
4. Verify configuration form displayed with fields:
   - API Key
   - Customer Number
   - Contract ID (optional)
   - Test Mode checkbox
5. Enter test credentials:
   - API Key: CPC_TEST_KEY
   - Customer Number: 123456789
   - Contract ID: (leave empty)
   - Test Mode: Yes
6. Click "Save Configuration"
7. Verify success notification displayed
8. Verify configuration saved and retrievable
```

**Pass Criteria**: 
- ✓ Form displays all Canada Post fields
- ✓ Configuration saves successfully
- ✓ Success message displayed
- ✓ Configuration persists after page refresh

---

#### UAT-SHIP-S1-002: Configure UPS
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S1-002 |
| Priority | High |

**Steps**:
```
1. Navigate to UPS configuration
2. Verify form fields:
   - OAuth Token
   - Shipper Number
   - Access License Number
   - Test Mode
3. Enter UPS credentials
4. Save configuration
5. Verify success
```

**Pass Criteria**: 
- ✓ UPS fields displayed correctly
- ✓ Configuration saved

---

#### UAT-SHIP-S1-003: Configure Multiple Carriers
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S1-003 |
| Priority | High |

**Steps**:
```
1. Configure Canada Post (per S1-001)
2. Configure UPS (per S1-002)
3. Configure FedEx
4. Configure DHL
5. Configure Purolator
6. Configure Canpar
7. Verify all configurations saved
8. Verify all configured carriers available for rate calculation
```

**Pass Criteria**: 
- ✓ All 6 carriers can be configured
- ✓ Configurations persist
- ✓ All carriers available for calculation

---

#### UAT-SHIP-S1-004: Edit Carrier Configuration
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S1-004 |
| Priority | Medium |

**Steps**:
```
1. Open existing Canada Post configuration
2. Change API Key to new value
3. Save configuration
4. Verify updated value saved
```

**Pass Criteria**: 
- ✓ Changes saved correctly
- ✓ Old value replaced with new

---

### 3.2 Scenario Set 2: Shipping Rate Calculation

#### UAT-SHIP-S2-001: Calculate Rates for Order
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S2-001 |
| Priority | Critical |
| Precondition | Carrier configured, test order exists |

**Steps**:
```
1. Create or open sales order
2. Verify customer shipping address entered:
   - Address: 123 Main Street
   - City: Toronto
   - Province: ON
   - Postal Code: M5V 1A1
   - Country: Canada
3. Verify order items with weights:
   - Item A: 2 units @ 1.5kg
   - Item B: 1 unit @ 3.0kg
4. Click "Calculate Shipping" button
5. Observe rate calculation in progress
6. Verify shipping quotes displayed in table:
   - Carrier column
   - Service column
   - Rate column
   - Transit Days column
   - Select button
```

**Test Data**:
```
Customer: ABC Corporation
Address: 123 Main St, Toronto, ON, M5V 1A1, Canada
Total Weight: 6kg (calculated from items)

Expected: Multiple carrier quotes displayed
```

**Pass Criteria**: 
- ✓ Quote table displayed
- ✓ Multiple carrier options shown
- ✓ All quote columns present
- ✓ Rates displayed in currency format

---

#### UAT-SHIP-S2-002: No Carriers Configured
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S2-002 |
| Priority | Medium |

**Steps**:
```
1. Remove or disable all carrier configurations
2. Attempt to calculate shipping
3. Observe system response
```

**Expected Result**: User-friendly message displayed

**Pass Criteria**: 
- ✓ Informative message shown
- ✓ No system errors
- ✓ Clear guidance to configure carriers

---

#### UAT-SHIP-S2-003: Rate Calculation with Missing Address
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S2-003 |
| Priority | Medium |

**Steps**:
```
1. Create order without customer address
2. Attempt to calculate shipping
3. Observe system response
```

**Expected Result**: Error message prompting for address

**Pass Criteria**: 
- ✓ Error message displayed
- ✓ No calculation attempted
- ✓ Clear guidance provided

---

### 3.3 Scenario Set 3: Quote Selection

#### UAT-SHIP-S3-001: Select Shipping Method
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S3-001 |
| Priority | Critical |

**Steps**:
```
1. Calculate shipping rates (per S2-001)
2. Review available options and rates
3. Select preferred shipping method (e.g., UPS Ground)
4. Click "Select" button for chosen option
5. Verify:
   - Shipping selection saved
   - Carrier name displayed on order
   - Service name displayed on order
   - Shipping cost added to order
   - Order total updated with shipping
```

**Test Data**:
```
Selected Option:
- Carrier: UPS
- Service: Ground
- Rate: $11.25

Expected Order Update:
- Subtotal: $150.00
- Shipping: $11.25
- Total: $161.25
```

**Pass Criteria**: 
- ✓ Selection saved successfully
- ✓ Shipping carrier name shown
- ✓ Shipping service shown
- ✓ Shipping cost added to order
- ✓ Order total recalculated

---

#### UAT-SHIP-S3-002: Compare Shipping Options
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S3-002 |
| Priority | High |

**Steps**:
```
1. Calculate shipping rates
2. Compare options side-by-side:
   - Rate
   - Transit time
   - Carrier reliability (staff knowledge)
3. Select option based on balance of cost and speed
```

**Pass Criteria**: 
- ✓ All options comparable
- ✓ Information clear for decision-making

---

#### UAT-SHIP-S3-003: Change Selected Shipping
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S3-003 |
| Priority | Medium |

**Steps**:
```
1. Select initial shipping method
2. View order with shipping
3. Decide to change shipping method
4. Recalculate or select different option
5. Verify shipping updated:
   - New carrier shown
   - New service shown
   - New cost reflected
   - Total updated
```

**Pass Criteria**: 
- ✓ Shipping can be changed
- ✓ Order updates reflect new selection
- ✓ Total recalculates correctly

---

### 3.4 Scenario Set 4: Order Integration

#### UAT-SHIP-S4-001: View Shipping on Order
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S4-001 |
| Priority | High |

**Steps**:
```
1. Select shipping for order
2. Save order
3. Close and reopen order
4. Verify shipping details displayed:
   - Carrier
   - Service
   - Cost
```

**Pass Criteria**: 
- ✓ Shipping persisted with order
- ✓ Shipping displayed on order view
- ✓ Data matches saved selection

---

#### UAT-SHIP-S4-002: Order Total with Shipping
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S4-002 |
| Priority | High |

**Steps**:
```
1. Create order with items totaling $200.00
2. Add shipping of $15.50
3. Verify order total = $215.50
4. Save order
5. Reopen order
6. Verify total still = $215.50
```

**Pass Criteria**: 
- ✓ Total includes shipping
- ✓ Total persists with order
- ✓ Total calculates correctly

---

#### UAT-SHIP-S4-003: Remove Shipping from Order
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S4-003 |
| Priority | Low |

**Steps**:
```
1. Open order with shipping
2. Remove/deselect shipping
3. Verify:
   - Shipping fields cleared
   - Shipping cost removed
   - Total reduced by shipping amount
```

**Pass Criteria**: 
- ✓ Shipping removed successfully
- ✓ Total updated

---

### 3.5 Scenario Set 5: Performance

#### UAT-SHIP-S5-001: Rate Calculation Speed
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S5-001 |
| Priority | Medium |

**Steps**:
```
1. Configure 2-3 carriers with test credentials
2. Calculate shipping for order
3. Measure time from button click to results displayed
4. Verify completes in reasonable time (< 10 seconds acceptable)
```

**Performance Target**: < 10 seconds for rate calculation

**Pass Criteria**: 
- ✓ Rates displayed within acceptable time
- ✓ Progress indicator shown during calculation
- ✓ No timeout errors

---

### 3.6 Scenario Set 6: Error Handling

#### UAT-SHIP-S6-001: Invalid Carrier Credentials
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S6-001 |
| Priority | Medium |

**Steps**:
```
1. Configure carrier with invalid credentials
2. Attempt to calculate shipping
3. Observe how errors are handled
```

**Expected Result**: Graceful error handling, other carriers still calculate

**Pass Criteria**: 
- ✓ Error message displayed
- ✓ Other carriers still return results (if valid)
- ✓ System remains stable

---

#### UAT-SHIP-S6-002: Carrier API Timeout
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S6-002 |
| Priority | Medium |

**Steps**:
```
1. Configure carrier
2. Simulate API timeout (if possible) or wait for timeout
3. Observe system behavior
```

**Expected Result**: Timeout handled gracefully

**Pass Criteria**: 
- ✓ System continues operation
- ✓ User notified of timeout
- ✓ Other carriers processed

---

### 3.7 Scenario Set 7: Security

#### UAT-SHIP-S7-001: Credential Display Security
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S7-001 |
| Priority | High |

**Steps**:
```
1. Configure carrier with credentials
2. Open configuration form for same carrier
3. Verify credentials displayed as masked/hidden
4. Verify API secrets not visible in plain text
```

**Pass Criteria**: 
- ✓ Sensitive fields use password/mask type
- ✓ No plain-text secrets displayed
- ✓ Credentials stored securely

---

#### UAT-SHIP-S7-002: Test Mode Configuration
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-SHIP-S7-002 |
| Priority | Medium |

**Steps**:
```
1. Enable test mode for carrier
2. Calculate shipping
3. Verify test/sandbox mode used
4. Verify no live charges incurred
```

**Pass Criteria**: 
- ✓ Test mode flag preserved
- ✓ Sandbox APIs called

---

## 4. Defect Tracking

### 4.1 Defect Severity Definitions

| Severity | Definition | Example |
|----------|------------|---------|
| Critical | System unusable, wrong rates charged | Rate calculation completely fails |
| High | Major function broken | Cannot save configuration |
| Medium | Function impaired | Slow calculation |
| Low | Cosmetic issue | Minor UI display issue |

### 4.2 Critical Test Data

| Test | Input | Expected Output | Verified |
|------|-------|-----------------|----------|
| TC-1 | Canada Post config (valid) | Config saved | ☐ |
| TC-2 | Canada Post config (invalid) | Validation error | ☐ |
| TC-3 | Rate calculation | Quotes returned | ☐ |
| TC-4 | Select shipping | Order updated | ☐ |

---

## 5. Sign-Off Criteria

### 5.1 Release Criteria

| Criterion | Target | Status |
|-----------|--------|--------|
| Critical scenarios pass | 100% | Required |
| High priority scenarios pass | 100% | Required |
| Configuration save verified | 100% | Required |
| Rate calculation verified | 100% | Required |
| Order integration verified | 100% | Required |
| Critical defects open | 0 | Required |
| High defects open | 0 | Required |

### 5.2 Configuration Verification Checklist

| Carrier | Configured | Validated | Verified |
|---------|------------|-----------|----------|
| Canada Post | ☐ | ☐ | ☐ |
| UPS | ☐ | ☐ | ☐ |
| FedEx | ☐ | ☐ | ☐ |
| DHL | ☐ | ☐ | ☐ |
| Purolator | ☐ | ☐ | ☐ |
| Canpar | ☐ | ☐ | ☐ |

---

## 6. Test Execution Timeline

| Phase | Activity | Duration |
|-------|----------|----------|
| Day 1 AM | UAT Planning & Setup | 4 hours |
| Day 1 PM | Carrier Configuration Testing | 4 hours |
| Day 2 AM | Rate Calculation Testing | 4 hours |
| Day 2 PM | Quote Selection & Order Integration | 4 hours |
| Day 3 AM | Performance & Error Handling | 4 hours |
| Day 3 PM | Security Verification | 4 hours |
| Day 4 | Final Regression & Sign-off | 4 hours |

---

## 7. Sign-Off Template

```
UAT Sign-Off Form - FA Shipping Module

Module: ksf_FA_Shipping
Version: 1.0.0
Test Period: [Start Date] - [End Date]

Test Summary:
- Total Scenarios: [X]
- Passed: [X]
- Failed: [X]
- Blocked: [X]

Critical/High Defects: [List or "None"]

Configuration Verification:
- Canada Post: [Configured/Not Configured]
- UPS: [Configured/Not Configured]
- FedEx: [Configured/Not Configured]
- DHL: [Configured/Not Configured]
- Purolator: [Configured/Not Configured]
- Canpar: [Configured/Not Configured]

Rate Calculation:
- Rates calculated successfully: [Yes/No]
- Multiple carriers working: [Yes/No]
- Performance acceptable: [Yes/No]

User Acceptance Decision:
[ ] APPROVED - Module meets acceptance criteria
[ ] APPROVED WITH CONDITIONS - Minor issues noted
[ ] REJECTED - Major issues require resolution

Comments:
_____________________________________________________________

Signee Name: _________________________
Role: _________________________
Date: _________________________
Signature: _________________________
```

---

## 8. Integration Verification

### 8.1 ksf_Shipping_Core Dependency

| Component | Status |
|-----------|--------|
| CarrierAdapters.php loads | ☐ |
| CanadianShippingCalculator available | ☐ |
| getQuotes() method callable | ☐ |
| Adapters registered correctly | ☐ |

### 8.2 FA Integration Points

| Point | Verification |
|-------|--------------|
| Company options storage | ☐ |
| Sales orders update | ☐ |
| Order total calculation | ☐ |

---

*Document Version: 1.0.0*
*Last Updated: 2026-05-13*
*Author: KSFII Development Team*