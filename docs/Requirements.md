# Requirements Document - KSF FA Shipping Module

## Integration with ksf_Shipping_Core

### Functional Requirements

#### FR-1: Shipping Methods Management
- **FR-1.1**: Admin must be able to create shipping methods via UI
- **FR-1.2**: Methods must be stored in fa_shipping_methods table
- **FR-1.3**: Admin must be able to configure method settings
- **FR-1.4**: Admin must be able to enable/disable methods

#### FR-2: Destination Management
- **FR-2.1**: Admin must be able to create shipping zones
- **FR-2.2**: Zones must support country/state/postcode filters
- **FR-2.3**: Wildcard matching for postcodes (e.g., "AB*")
- **FR-2.4**: Methods can be assigned to multiple zones

#### FR-3: Order Integration
- **FR-3.1**: Shipping calculator must run during order entry
- **FR-3.2**: Available rates must be displayed to customer
- **FR-3.3**: Selected shipping method must be saved with order
- **FR-3.4**: Shipping cost must be added to order total

#### FR-4: FA Hooks Integration
- **FR-4.1**: Use FA's activate_extension() for module activation
- **FR-4.2**: Use update_databases() with SQL files
- **FR-4.3**: Hook into FA's order validation for shipping calculation
- **FR-4.4**: Hook into FA's delivery/shipping screens

### Non-Functional Requirements

#### NFR-1: Compatibility
- **NPR-1.1**: Module must work with FA 2.4.x
- **NPR-1.2**: PHP 7.3+ compatibility
- **NPR-1.3**: Follow FA coding standards

#### NFR-2: Performance
- **NPR-2.1**: Rate calculation must complete in <200ms for 10 packages
- **NPR-2.2**: Cache shipping zones and methods
