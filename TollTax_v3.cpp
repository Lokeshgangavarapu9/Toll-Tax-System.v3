/*
 ============================================================================
  TOLL TAX MANAGEMENT SYSTEM — v3.0  "STOP-FREE AI AUTOMATION EDITION"
  OOP Project | SRM University-AP | C++ (C++11 or later)

  Team:
    Finance & Marketing Lead  : Lokesh
    Technical Lead            : Mathews
    Core Team                 : Jishnu, Rohit, Greysi

  ── What's New in v3.0 ──────────────────────────────────────────────────────
    Feature A │ CameraSystem   — Dual-Camera ALPR plate-tamper detection
    Feature B │ Database       — Central govt registry (std::map lookup)
    Feature C │ FraudDetector  — AI weight-vs-class mismatch engine
    Feature D │ Notification   — Auto SMS + Push receipt after clearance
    + All v2.0 features preserved (FASTag, Passes, Surge, Weight Penalty, etc.)

  Compile : g++ -std=c++11 -o TollTax_v3 TollTax_v3.cpp
  Run     : ./TollTax_v3

  Note    : Box-drawing characters require a UTF-8 terminal.
            On Windows, run:  chcp 65001  before executing.
 ============================================================================
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <algorithm>
using namespace std;

// ============================================================================
//  SECTION 0 — UTILITY FUNCTIONS
//  (Shared across all classes; declared first so every class can use them.)
// ============================================================================

/** Returns the current local hour (0-23). */
int getCurrentHour() {
    time_t now = time(nullptr);
    tm* lt = localtime(&now);
    return lt->tm_hour;
}

/** Returns a formatted timestamp string: "YYYY-MM-DD HH:MM:SS". */
string getCurrentTimestamp() {
    time_t now = time(nullptr);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buf);
}

/** Peak hours: 08:00-09:59 (morning) OR 17:00-19:59 (evening). */
bool isPeakHour() {
    int h = getCurrentHour();
    return (h >= 8 && h < 10) || (h >= 17 && h < 20);
}

void divider()     { cout << "  ----------------------------------------\n"; }
void dividerWide() { cout << "  ========================================\n"; }
void dividerDbl()  { cout << "  ########################################\n"; }

// ============================================================================
//  SECTION 1 — ENUMERATIONS
// ============================================================================

/** Pass subscription tier for toll waiver. */
enum class PassType { NONE, DAILY, MONTHLY };

string passTypeName(PassType p) {
    if (p == PassType::DAILY)   return "Daily Pass";
    if (p == PassType::MONTHLY) return "Monthly Pass";
    return "None";
}

/** Status codes stored in the central government database. */
enum class VehicleStatus {
    CLEAR,          // No issues — proceed
    STOLEN,         // Reported stolen — lock barrier, alert police
    UNPAID_FINES    // Outstanding e-challan fines — allow entry, issue notice
};

// ============================================================================
//  SECTION 2 — STRUCT: VehicleRecord
//  Plain data container used as the value type inside Database's std::map.
//  Aggregates all fields a government record would hold.
// ============================================================================
struct VehicleRecord {
    string        plate;
    string        ownerName;
    VehicleStatus status;
    double        unpaidFineAmount;  // 0.0 when CLEAR or STOLEN

    // Default constructor — produces an unknown "Guest" record
    VehicleRecord()
        : plate(""), ownerName("Unknown Guest"),
          status(VehicleStatus::CLEAR), unpaidFineAmount(0.0) {}

    // Parameterised constructor
    VehicleRecord(const string& p, const string& name,
                  VehicleStatus s, double fine = 0.0)
        : plate(p), ownerName(name), status(s), unpaidFineAmount(fine) {}
};

// ============================================================================
//  SECTION 3 — CLASS: Database
//
//  OOP Concept : Encapsulation — all record data is private inside a std::map.
//  Data Structure: std::map<string, VehicleRecord> gives O(log n) lookup.
//
//  Simulates a real-time NIC (National Informatics Centre) vehicle registry.
//  Pre-seeded with team plates (CLEAR) + flagged plates for demo.
// ============================================================================
class Database {
private:
    // OOP: private data — only accessible through public interface below
    map<string, VehicleRecord> records;  // key = normalised license plate

public:
    /** Seed constructor: loads mock data into the in-memory database. */
    Database() {
        // ── Team member plates — always CLEAR ───────────────────────────────
        records["AP09AB1234"] = VehicleRecord("AP09AB1234", "Lokesh",
                                               VehicleStatus::CLEAR, 0.0);
        records["AP09CD5678"] = VehicleRecord("AP09CD5678", "Mathews",
                                               VehicleStatus::CLEAR, 0.0);
        records["AP09EF9012"] = VehicleRecord("AP09EF9012", "Jishnu",
                                               VehicleStatus::CLEAR, 0.0);
        records["AP09GH3456"] = VehicleRecord("AP09GH3456", "Rohit",
                                               VehicleStatus::CLEAR, 0.0);
        records["AP09IJ7890"] = VehicleRecord("AP09IJ7890", "Greysi",
                                               VehicleStatus::CLEAR, 0.0);

        // ── Stolen vehicles — barrier MUST remain closed ─────────────────────
        records["TN22GH9999"] = VehicleRecord("TN22GH9999", "Unknown",
                                               VehicleStatus::STOLEN, 0.0);
        records["MH12PQ5500"] = VehicleRecord("MH12PQ5500", "Sundar K.",
                                               VehicleStatus::STOLEN, 0.0);
        records["GJ05RT2288"] = VehicleRecord("GJ05RT2288", "Anonymous",
                                               VehicleStatus::STOLEN, 0.0);

        // ── Vehicles with outstanding e-challan fines ────────────────────────
        records["KA05MN3344"] = VehicleRecord("KA05MN3344", "Rajan S.",
                                               VehicleStatus::UNPAID_FINES, 850.0);
        records["TS08PQ7777"] = VehicleRecord("TS08PQ7777", "Priya M.",
                                               VehicleStatus::UNPAID_FINES, 200.0);
        records["DL07XY1122"] = VehicleRecord("DL07XY1122", "Aman V.",
                                               VehicleStatus::UNPAID_FINES, 1200.0);
    }

    /**
     * Looks up a license plate in the government registry.
     * If the plate is unknown (first-time visitor), returns a default
     * CLEAR guest record rather than throwing — mirrors real-world behaviour
     * where unregistered vehicles are still allowed but logged.
     */
    VehicleRecord lookup(const string& plate) const {
        // std::map::find returns end() when key is absent
        auto it = records.find(plate);
        if (it != records.end()) return it->second;

        // Guest / unregistered vehicle
        VehicleRecord guest;
        guest.plate     = plate;
        guest.ownerName = "Guest (Unregistered)";
        guest.status    = VehicleStatus::CLEAR;
        return guest;
    }

    /** Registers a new vehicle (called from Wallet Manager when new FASTag added). */
    void addRecord(const VehicleRecord& rec) {
        records[rec.plate] = rec;
    }

    /** Clears outstanding fines after payment (called after fine settlement). */
    void clearFines(const string& plate) {
        auto it = records.find(plate);
        if (it != records.end()) {
            it->second.status           = VehicleStatus::CLEAR;
            it->second.unpaidFineAmount = 0.0;
            cout << "  [DATABASE] Fines cleared for plate: " << plate << "\n";
        }
    }

    /** Prints a summary of the database contents. */
    void printSummary() const {
        int stolen = 0, fined = 0, clear = 0;
        for (const auto& kv : records) {
            switch (kv.second.status) {
                case VehicleStatus::STOLEN:       ++stolen; break;
                case VehicleStatus::UNPAID_FINES: ++fined;  break;
                default:                           ++clear;  break;
            }
        }
        cout << "\n  ===== Central Government Database =====\n"
             << "  Total registered plates : " << records.size() << "\n"
             << "  Status breakdown:\n"
             << "    CLEAR        : " << clear  << "\n"
             << "    STOLEN       : " << stolen << "\n"
             << "    UNPAID FINES : " << fined  << "\n";
    }
};

// ============================================================================
//  SECTION 4 — CLASS: CameraSystem   (Feature A — Dual-Camera ALPR)
//
//  OOP Concept : Encapsulation — camera readings are private; only the
//                verified plate is exposed via public getter.
//
//  Simulates two physical IP cameras mounted at the toll plaza:
//    Camera 1 (front)  — faces the approaching vehicle's front plate.
//    Camera 2 (rear)   — mounted on a side post, reads the rear plate
//                        once the vehicle aligns with the weight sensor.
//
//  Anti-tamper logic: if front plate != rear plate, it means the driver
//  has physically covered/swapped a plate — barrier stays LOCKED.
//
//  Tamper simulation: rear camera has a 10 % random error rate to model
//  real-world OCR noise and allow demo of the security path.
// ============================================================================
class CameraSystem {
private:
    string frontPlate;    // OCR reading from Camera 1
    string rearPlate;     // OCR reading from Camera 2
    bool   scanComplete;

    /** Normalises a plate string to UPPERCASE for consistent comparison. */
    static string normalise(const string& s) {
        string out = s;
        for (char& c : out) c = toupper(static_cast<unsigned char>(c));
        return out;
    }

    /**
     * Simulates Camera 2's OCR with a 10 % error / tamper chance.
     * When the error fires, one character in the plate is mutated,
     * representing either a physical plate swap or a dirty lens read.
     */
    string simulateRearRead(const string& truePlate) const {
        if ((rand() % 10) == 0) {           // 10 % tamper simulation
            string corrupted = truePlate;
            size_t pos = static_cast<size_t>(rand()) % corrupted.size();
            // Flip one character digit/letter so the plates differ
            char& ch = corrupted[pos];
            if (isdigit(ch)) ch = (ch == '9') ? '0' : ch + 1;
            else              ch = (ch == 'Z') ? 'A' : ch + 1;
            return corrupted;   // tampered / misread plate
        }
        return truePlate;       // 90 % — cameras agree, all good
    }

public:
    CameraSystem() : scanComplete(false) {}

    /**
     * Fires both cameras.
     * userPlate    = plate the operator types / front camera reads
     * allowTamper  = true in normal mode (enables 10 % simulation)
     *                false when we want a guaranteed-clean scan
     * Returns true if both cameras read the same plate (no tampering).
     */
    bool scan(const string& userPlate, bool allowTamper = true) {
        frontPlate   = normalise(userPlate);
        rearPlate    = allowTamper ? simulateRearRead(frontPlate) : frontPlate;
        scanComplete = true;

        cout << "\n  [CAM-1 FRONT]  Read : " << frontPlate
             << "\n  [CAM-2 REAR ]  Read : " << rearPlate  << "\n";

        return (frontPlate == rearPlate);   // true = plates match = safe
    }

    /** Returns the verified plate (always use front camera's reading). */
    string getVerifiedPlate() const { return frontPlate; }
};

// ============================================================================
//  SECTION 5 — CLASS: FraudDetector   (Feature C — Weight Mismatch AI)
//
//  OOP Concept : Static utility class — no instance state needed.
//                All methods are static; the class acts as a namespace.
//
//  Cross-references declared vehicle type against physical weight sensor
//  data. Weight bounds are derived from real-world MORTH (Ministry of Road
//  Transport) gross vehicle weight specifications.
// ============================================================================
class FraudDetector {
public:
    /** Holds the permissible weight band for a vehicle class (in metric tons). */
    struct WeightBounds {
        double minTons;
        double maxTons;
    };

    /**
     * Returns the legal weight band for a given vehicle type string.
     * Any weight outside this range for the declared class triggers a fraud alert.
     */
    static WeightBounds getBounds(const string& vehicleType) {
        // Ranges based on MORTH / IS 9661 standards + reasonable tolerance
        if (vehicleType == "Bike")      return { 0.05,  0.50 };
        if (vehicleType == "Car")       return { 0.80,  3.50 };
        if (vehicleType == "Bus")       return { 5.00, 18.00 };
        if (vehicleType == "Truck")     return { 5.00, 40.00 };
        if (vehicleType == "Ambulance") return { 2.50,  7.00 };
        return { 0.0, 999.0 };  // unknown type — no restriction applied
    }

    /**
     * Core fraud-check gate.
     * Returns true if the declared weight is OUTSIDE the expected band,
     * i.e., if FRAUD is detected.
     */
    static bool isFraud(const string& vehicleType, double weightTons) {
        WeightBounds b = getBounds(vehicleType);
        return (weightTons < b.minTons || weightTons > b.maxTons);
    }

    /** Prints a formatted AI fraud alert to the terminal + appends fraud_log.txt. */
    static void printFraudAlert(const string& vehicleType,
                                double        weightTons,
                                const string& plate) {
        WeightBounds b = getBounds(vehicleType);
        cout << "\n  ╔══════════════════════════════════════════╗\n"
             << "  ║       *** AI FRAUD ALERT RAISED ***      ║\n"
             << "  ╚══════════════════════════════════════════╝\n"
             << "  Plate         : " << plate << "\n"
             << "  Declared type : " << vehicleType << "\n"
             << "  Sensor weight : " << fixed << setprecision(2)
             << weightTons << " tons\n"
             << "  Legal range   : " << b.minTons
             << " – "  << b.maxTons << " tons\n"
             << "  AI Decision   : MISMATCH — BARRIER LOCKED\n"
             << "  [LOG] Incident written to fraud_log.txt\n";

        // Persist incident for later review
        ofstream f("fraud_log.txt", ios::app);
        if (f.is_open()) {
            f << "[" << getCurrentTimestamp() << "]"
              << "  FRAUD | Plate: " << plate
              << " | Type: " << vehicleType
              << " | Weight: " << weightTons << "t"
              << " | Legal: " << b.minTons << "-" << b.maxTons << "t\n";
        }
    }

    /** Prints an "all clear" weight confirmation. */
    static void printWeightOK(const string& vehicleType, double weightTons) {
        cout << "  [AI-WEIGHT]   " << vehicleType
             << " @ " << fixed << setprecision(2) << weightTons
             << " tons — within legal range. PASS.\n";
    }
};

// ============================================================================
//  SECTION 6 — CLASS: Notification   (Feature D — Auto Receipt System)
//
//  OOP Concept : Static utility class with overloaded print methods.
//
//  In a production system these methods would call:
//    sendSMS   → Twilio / MSG91 SMS gateway API
//    sendPush  → Firebase Cloud Messaging (FCM)
//    sendAlert → Police / CCTNS API
//  Here they print formatted terminal receipts as simulation.
// ============================================================================
class Notification {
public:
    /**
     * Simulates dispatching an SMS receipt to the registered mobile number.
     * Called automatically after a successful FASTag deduction.
     */
    static void sendSMS(const string& ownerName,
                        const string& plate,
                        const string& vehicleType,
                        double        tollAmount,
                        double        walletBalance,
                        bool          isFasTag) {
        // Pre-format doubles to avoid setw/setprecision interaction issues
        ostringstream tollStr, balStr;
        tollStr << fixed << setprecision(2) << "Rs." << tollAmount;
        balStr  << fixed << setprecision(2) << "Rs." << walletBalance;

        cout << "\n  ┌──────────────────────────────────────────┐\n"
             << "  │         [SMS NOTIFICATION SENT]          │\n"
             << "  ├──────────────────────────────────────────┤\n"
             << "  │  To       : " << ownerName << "\n"
             << "  │  Vehicle  : " << plate << "\n"
             << "  │  Type     : " << vehicleType << "\n"
             << "  │  Toll     : " << tollStr.str() << "\n";

        if (isFasTag)
            cout << "  │  FASTag   : " << balStr.str() << " remaining\n";
        else
            cout << "  │  Payment  : Cash collected at booth\n";

        cout << "  │  Time     : " << getCurrentTimestamp() << "\n"
             << "  │  Status   : PAID  ✓  BARRIER OPENED\n"
             << "  └──────────────────────────────────────────┘\n";
    }

    /**
     * Simulates dispatching a mobile push notification via FCM.
     * Appears on the owner's registered FASTag app.
     */
    static void sendPush(const string& ownerName,
                         const string& plate,
                         double        toll) {
        ostringstream tollStr;
        tollStr << fixed << setprecision(2) << toll;

        cout << "\n  ╔══════════════════════════════════════════╗\n"
             << "  ║      [PUSH NOTIFICATION SENT]            ║\n"
             << "  ╠══════════════════════════════════════════╣\n"
             << "  ║  Hi " << ownerName << "!\n"
             << "  ║  Toll of Rs." << tollStr.str()
             <<            " deducted for " << plate << "\n"
             << "  ║  Toll plaza cleared successfully.\n"
             << "  ║  Drive safe! — FASTag India\n"
             << "  ╚══════════════════════════════════════════╝\n";
    }

    /**
     * Dispatches a security alert to the on-duty toll officer and
     * logs the incident to security_alerts.txt.
     */
    static void sendSecurityAlert(const string& reason, const string& plate) {
        cout << "\n  !! [SECURITY ALERT — OFFICER NOTIFIED] !!\n"
             << "  Reason  : " << reason << "\n"
             << "  Plate   : " << plate  << "\n"
             << "  Action  : BARRIER LOCKED — Officer dispatched\n"
             << "  Time    : " << getCurrentTimestamp() << "\n";

        ofstream f("security_alerts.txt", ios::app);
        if (f.is_open()) {
            f << "[" << getCurrentTimestamp() << "]"
              << "  SECURITY ALERT | Reason: " << reason
              << " | Plate: " << plate << "\n";
        }
    }

    /**
     * Sends an outstanding-fine payment reminder to the vehicle owner.
     * Links to the official e-challan portal.
     */
    static void sendFineNotice(const string& ownerName,
                               const string& plate,
                               double        fineAmount) {
        ostringstream fineStr;
        fineStr << fixed << setprecision(2) << fineAmount;

        cout << "\n  [FINE NOTICE]\n"
             << "  To      : " << ownerName << "\n"
             << "  Plate   : " << plate << "\n"
             << "  Fine    : Rs." << fineStr.str() << " outstanding\n"
             << "  Pay at  : https://echallan.parivahan.gov.in\n"
             << "  Note    : Entry allowed. Fine must be cleared within 30 days.\n";
    }
};

// ============================================================================
//  SECTION 7 — CLASS: Wallet   (v2.0 FASTag — unchanged, fully preserved)
//
//  OOP Concept : Encapsulation — balance is private; only deduct/topUp
//                methods can modify it.
// ============================================================================
class Wallet {
private:
    string ownerName;
    string vehicleNumber;
    double balance;

public:
    // Decoupled constructor — no cin inside (OOP best practice)
    Wallet(const string& name, const string& vNum, double initialBalance)
        : ownerName(name), vehicleNumber(vNum), balance(initialBalance) {}

    // --- Getters (Feature 8: strict encapsulation) ---
    string getOwnerName()     const { return ownerName;     }
    string getVehicleNumber() const { return vehicleNumber; }
    double getBalance()       const { return balance;       }

    /** Deducts amount from wallet. Returns true on success. */
    bool deduct(double amount) {
        if (balance >= amount) { balance -= amount; return true; }
        return false;
    }

    /** Adds credit to the wallet. */
    void topUp(double amount) {
        balance += amount;
        cout << fixed << setprecision(2)
             << "  [Wallet] Rs." << amount
             << " added. New balance: Rs." << balance << "\n";
    }

    void display() const {
        cout << fixed << setprecision(2)
             << "  Owner: "    << ownerName
             << " | Plate: "   << vehicleNumber
             << " | Balance: Rs." << balance << "\n";
    }
};

// ============================================================================
//  SECTION 8 — ABSTRACT BASE CLASS: Vehicle
//
//  OOP Concepts:
//    Abstraction   — computeBaseToll() and getVehicleType() are pure virtual.
//    Encapsulation — totalCollection is private static with getter.
//    Inheritance   — Car, Bus, Truck, Bike, Ambulance all inherit this.
//    Polymorphism  — calculateToll() is virtual; Ambulance overrides it.
//
//  v3.0 additions:
//    • plateNumber field (protected — visible to Ambulance override)
//    • ownerName + autoBill params in calculateToll
//    • Notification calls embedded in calculateToll flow
// ============================================================================
class Vehicle {
    // ── Feature 8: strict encapsulation on static collection total ───────────
private:
    static double         totalCollection;  // only via getTotalCollection()
    static vector<string> exemptLog;        // Feature 4: ambulance analytics

    // ── Protected — accessible by all derived classes ─────────────────────
protected:
    int      distance;         // km travelled
    PassType pass;             // subscription pass tier
    bool     returnJourney;    // two-way pass flag
    double   surgeMultiplier;  // 1.0 normal | 1.5 peak surge
    string   plateNumber;      // v3.0: plate stored with vehicle object

public:
    // Feature 9: decoupled constructor — all inputs passed from main()
    Vehicle(int dist, PassType p, bool ret, const string& plate)
        : distance(dist), pass(p), returnJourney(ret), plateNumber(plate) {
        surgeMultiplier = isPeakHour() ? 1.5 : 1.0;  // Feature 3
    }

    virtual double computeBaseToll() const = 0;      // pure virtual
    virtual string getVehicleType()  const = 0;      // pure virtual

    string getPlate() const { return plateNumber; }

    /**
     * Master toll calculation — v3.0 enhanced.
     *
     * wallet    = nullptr → cash; pointer → FASTag auto-deduction
     * ownerName = resolved from Database lookup (used in notifications)
     * autoBill  = true  → v3.0 auto-deduct without manual confirmation
     *             false → v2.0 behaviour (show prompt if needed)
     *
     * Flow:
     *   Pass check → Return-journey multiplier → Peak-surge multiplier
     *   → FASTag deduction → Notification → File log
     */
    virtual void calculateToll(Wallet*       wallet    = nullptr,
                               const string& ownerName = "Vehicle Owner",
                               bool          autoBill  = false) {
        double toll = computeBaseToll();

        cout << "\n  Vehicle     : " << getVehicleType()
             << "\n  Plate       : " << plateNumber
             << "\n  Distance    : " << distance << " km"
             << "\n  Base Toll   : Rs." << fixed << setprecision(2) << toll;

        // Feature 6: Tiered pass — waives toll completely ────────────────────
        if (pass != PassType::NONE) {
            cout << "\n  Pass Active : " << passTypeName(pass)
                 << " — Toll WAIVED!\n";
            logTransaction(0.0);
            // Still send notification for pass holders
            if (wallet)
                Notification::sendSMS(ownerName, plateNumber, getVehicleType(),
                                      0.0, wallet->getBalance(), true);
            return;
        }

        // Feature 7: Return journey — 1.5x base fare charged upfront ─────────
        if (returnJourney) {
            toll *= 1.5;
            cout << "\n  Return Pass : 1.5x applied -> Rs." << toll;
        }

        // Feature 3: Peak-hour dynamic surge pricing ──────────────────────────
        if (surgeMultiplier > 1.0) {
            toll *= surgeMultiplier;
            cout << "\n  Peak Surge  : 1.5x applied -> Rs." << toll;
        }

        cout << "\n  Final Toll  : Rs." << toll << "\n";

        // ── v3.0 Feature D: Auto-Billing ─────────────────────────────────────
        if (wallet != nullptr) {
            if (autoBill)
                cout << "  [AUTO-BILL] FASTag deduction initiated...\n";

            if (wallet->deduct(toll)) {
                cout << "  [FASTag OK] Rs." << toll
                     << " deducted | Balance: Rs."
                     << wallet->getBalance() << "\n";
                // Trigger both notification channels automatically
                Notification::sendSMS(ownerName, plateNumber, getVehicleType(),
                                      toll, wallet->getBalance(), true);
                Notification::sendPush(ownerName, plateNumber, toll);
            } else {
                cout << "  [FASTag LOW] Insufficient balance (Rs."
                     << wallet->getBalance() << ")."
                     << " Collecting Rs." << toll << " in cash.\n";
                // Cash fallback notification
                Notification::sendSMS(ownerName, plateNumber, getVehicleType(),
                                      toll, 0.0, false);
            }
        } else {
            cout << "  [Cash] Toll Rs." << toll << " collected at booth.\n";
            Notification::sendSMS(ownerName, plateNumber, getVehicleType(),
                                  toll, 0.0, false);
        }

        totalCollection += toll;   // Feature 8: update private static
        logTransaction(toll);      // Feature 1: persist to file
    }

    // Feature 1: Append a timestamped entry to vehicle_log.txt
    void logTransaction(double toll) const {
        ofstream logFile("vehicle_log.txt", ios::app);
        if (logFile.is_open()) {
            logFile << "[" << getCurrentTimestamp() << "]"
                    << "  Plate: "   << plateNumber
                    << "  Type: "    << getVehicleType()
                    << "  Dist: "    << distance << "km"
                    << "  Pass: "    << passTypeName(pass)
                    << "  Return: "  << (returnJourney ? "Yes" : "No")
                    << "  Peak: "    << (surgeMultiplier > 1.0 ? "Yes" : "No")
                    << "  Toll: Rs." << fixed << setprecision(2) << toll << "\n";
        }
    }

    // Feature 4: Add an exempt-vehicle entry to the analytics vector
    static void logExemptVehicle(const string& entry) {
        exemptLog.push_back(entry);
    }

    static void showExemptLog() {
        cout << "\n===== Exempt Vehicle Analytics Log =====\n";
        if (exemptLog.empty())
            cout << "  No exempt vehicles recorded this session.\n";
        else
            for (size_t i = 0; i < exemptLog.size(); i++)
                cout << "  " << (i + 1) << ". " << exemptLog[i] << "\n";
    }

    // Feature 8: public getter for private static totalCollection
    static double getTotalCollection() { return totalCollection; }

    static void showTotal() {
        cout << fixed << setprecision(2)
             << "\n  Total Toll Collected This Session: Rs."
             << totalCollection << "\n";
    }

    // Feature 1: Persist session total to daily_collection.txt
    static void saveCollectionToFile() {
        ofstream f("daily_collection.txt", ios::app);
        if (f.is_open()) {
            f << "[" << getCurrentTimestamp() << "]"
              << "  Session Total: Rs."
              << fixed << setprecision(2) << totalCollection << "\n";
            cout << "  Session total saved to 'daily_collection.txt'.\n";
        } else {
            cout << "  ERROR: Could not open daily_collection.txt\n";
        }
    }

    virtual ~Vehicle() {}
};

// Static member definitions (required outside the class body in C++11)
double         Vehicle::totalCollection = 0.0;
vector<string> Vehicle::exemptLog;

// ============================================================================
//  SECTION 9 — DERIVED VEHICLE CLASSES
//  Each overrides computeBaseToll() and getVehicleType().
//  Constructors now accept plateNumber (v3.0 addition).
// ============================================================================

// ── Car: Rs. 2 per km ────────────────────────────────────────────────────────
class Car : public Vehicle {
public:
    Car(int dist, PassType p, bool ret, const string& plate)
        : Vehicle(dist, p, ret, plate) {}

    double computeBaseToll() const override { return distance * 2.0; }
    string getVehicleType()  const override { return "Car"; }
};

// ── Bus: Rs. 4 per km ────────────────────────────────────────────────────────
class Bus : public Vehicle {
public:
    Bus(int dist, PassType p, bool ret, const string& plate)
        : Vehicle(dist, p, ret, plate) {}

    double computeBaseToll() const override { return distance * 4.0; }
    string getVehicleType()  const override { return "Bus"; }
};

// ── Truck: Rs. 5 per km + weight penalty (Feature 2) ─────────────────────────
class Truck : public Vehicle {
private:
    double weightTons;

    // Class constants — defined once, shared across all Truck instances
    static const double WEIGHT_LIMIT_TONS;   // free-limit = 10 t
    static const double PENALTY_PER_TON;     // Rs. 50 per excess ton

public:
    Truck(int dist, double weight, PassType p, bool ret, const string& plate)
        : Vehicle(dist, p, ret, plate), weightTons(weight) {}

    double computeBaseToll() const override {
        double toll = distance * 5.0;

        // Feature 2: weight-based penalty calculation
        if (weightTons > WEIGHT_LIMIT_TONS) {
            double excess   = weightTons - WEIGHT_LIMIT_TONS;
            double penalty  = excess * PENALTY_PER_TON;
            cout << "\n  Weight      : " << weightTons << " tons"
                 << "\n  Excess      : " << excess    << " tons"
                 << "\n  Penalty     : Rs." << penalty;
            toll += penalty;
        } else {
            cout << "\n  Weight      : " << weightTons << " tons (within limit)";
        }
        return toll;
    }

    string getVehicleType() const override {
        ostringstream oss;
        oss << "Truck (" << fixed << setprecision(1) << weightTons << "t)";
        return oss.str();
    }
};

const double Truck::WEIGHT_LIMIT_TONS = 10.0;
const double Truck::PENALTY_PER_TON   = 50.0;

// ── Bike: Rs. 1 per km ───────────────────────────────────────────────────────
class Bike : public Vehicle {
public:
    Bike(int dist, PassType p, bool ret, const string& plate)
        : Vehicle(dist, p, ret, plate) {}

    double computeBaseToll() const override { return distance * 1.0; }
    string getVehicleType()  const override { return "Bike"; }
};

// ── Ambulance: always free — Feature 4 analytics override ────────────────────
class Ambulance : public Vehicle {
public:
    // Ambulance ignores pass type and return journey — always exempt
    Ambulance(int dist, const string& plate)
        : Vehicle(dist, PassType::NONE, false, plate) {}

    double computeBaseToll() const override { return 0.0; }
    string getVehicleType()  const override { return "Ambulance"; }

    // Full override: provides exempt-specific output + analytics logging
    void calculateToll(Wallet*       /*unused*/ = nullptr,
                       const string& /*unused*/ = "Emergency Services",
                       bool          /*unused*/ = false) override {
        string entry = "[" + getCurrentTimestamp() + "]"
                     + "  Ambulance | Plate: " + plateNumber
                     + " | Dist: " + to_string(distance)
                     + " km | Toll: Rs.0.00 (Emergency Exempt)";

        logExemptVehicle(entry);    // Feature 4: vector analytics
        logTransaction(0.0);        // Feature 1: persist to file

        cout << "\n  Vehicle     : Ambulance (Emergency Service)"
             << "\n  Plate       : " << plateNumber
             << "\n  Toll        : Rs.0.00 — Fully Exempt"
             << "\n  [Analytics] Passage logged to exempt log.\n";

        // Notification still sent for dispatch-record purposes
        Notification::sendSMS("Emergency Services", plateNumber,
                               "Ambulance", 0.0, 0.0, false);
    }
};

// ============================================================================
//  SECTION 10 — HELPER FUNCTIONS
// ============================================================================

/** Prompts operator to choose a subscription pass tier. */
PassType choosePass() {
    cout << "  Pass? (0=None  1=Daily  2=Monthly): ";
    int p; cin >> p;
    if (p == 1) return PassType::DAILY;
    if (p == 2) return PassType::MONTHLY;
    return PassType::NONE;
}

/**
 * Searches for a wallet whose registered plate matches the scanned plate.
 * This enables TRUE auto-billing — no manual selection required.
 * Returns nullptr if no FASTag is linked to this plate.
 */
Wallet* findWalletByPlate(vector<Wallet>& wallets, const string& plate) {
    for (auto& w : wallets)
        if (w.getVehicleNumber() == plate) return &w;
    return nullptr;
}

/** Manual wallet selector — fallback for unregistered vehicles. */
Wallet* chooseWallet(vector<Wallet>& wallets) {
    if (wallets.empty()) return nullptr;
    cout << "  Use FASTag manually? (0=Cash  1=Yes): ";
    int use; cin >> use;
    if (!use) return nullptr;
    cout << "  Select wallet:\n";
    for (size_t i = 0; i < wallets.size(); i++) {
        cout << "    " << (i + 1) << ". ";
        wallets[i].display();
    }
    int idx; cin >> idx;
    if (idx >= 1 && idx <= static_cast<int>(wallets.size()))
        return &wallets[idx - 1];
    return nullptr;
}

// ============================================================================
//  SECTION 11 — AI PIPELINE FUNCTION
//
//  This function IS the "Stop-Free AI Automation" engine.
//  It is called for EVERY vehicle BEFORE any toll is calculated.
//
//  The pipeline is a strict 3-stage gate:
//    Stage 1 — CameraSystem: dual-plate ALPR tamper check
//    Stage 2 — Database    : stolen / unpaid-fines registry lookup
//    Stage 3 — FraudDetector: weight-vs-class mismatch detection
//
//  If ANY stage fails, the function returns false and the barrier
//  stays LOCKED. The toll calculation is never reached.
//
//  Parameters:
//    plate       — the plate entered by the operator
//    vehicleType — "Car", "Bus", "Truck", "Bike", "Ambulance"
//    weightTons  — sensor weight in metric tons
//    db          — reference to the central database instance
//    outOwnerName— [out] filled with the owner name from DB for notifications
//
//  Returns:
//    true  — all checks passed, proceed to toll calculation
//    false — one or more checks failed, barrier remains closed
// ============================================================================
bool runAIPipeline(const string& plate,
                   const string& vehicleType,
                   double        weightTons,
                   Database&     db,
                   string&       outOwnerName) {
    dividerDbl();
    cout << "  [AI PIPELINE] Starting verification for : " << plate << "\n";
    cout << "  [AI PIPELINE] Timestamp                 : "
         << getCurrentTimestamp() << "\n";
    dividerDbl();

    // =========================================================
    // STAGE 1 — Dual-Camera ALPR (CameraSystem)
    // =========================================================
    cout << "\n  STAGE 1  ► Dual-Camera ALPR Verification\n";
    divider();

    CameraSystem cam;
    bool platesMatch = cam.scan(plate, true);  // true = enable tamper sim

    if (!platesMatch) {
        cout << "  [ALPR] *** PLATE MISMATCH DETECTED ***\n"
             << "  [ALPR] Front: " << plate
             << "  vs  Rear: different reading.\n"
             << "  [ALPR] Possible plate swap / tampering. BARRIER LOCKED.\n";
        Notification::sendSecurityAlert("License Plate Mismatch (Tampering)", plate);
        return false;
    }

    cout << "  [ALPR] Both cameras AGREE. Verified plate: "
         << cam.getVerifiedPlate() << "  ✓\n";
    string verifiedPlate = cam.getVerifiedPlate();

    // =========================================================
    // STAGE 2 — Central Government Database Lookup
    // =========================================================
    cout << "\n  STAGE 2  ► Central Government Database Check\n";
    divider();

    VehicleRecord rec = db.lookup(verifiedPlate);
    outOwnerName = rec.ownerName;

    cout << "  [DATABASE] Owner  : " << rec.ownerName << "\n"
         << "  [DATABASE] Plate  : " << rec.plate << "\n";

    if (rec.status == VehicleStatus::STOLEN) {
        cout << "  [DATABASE] Status : *** STOLEN VEHICLE FLAGGED ***\n";
        Notification::sendSecurityAlert("Stolen Vehicle Identified", verifiedPlate);
        return false;   // hard block — barrier stays closed
    }

    if (rec.status == VehicleStatus::UNPAID_FINES) {
        cout << "  [DATABASE] Status : UNPAID FINES — Rs."
             << fixed << setprecision(2) << rec.unpaidFineAmount << "\n";
        Notification::sendFineNotice(rec.ownerName, verifiedPlate,
                                     rec.unpaidFineAmount);
        // Soft block — vehicle is allowed through but fine notice is served.
        // Change this `return false` to block such vehicles at your booth.
        cout << "  [DATABASE] Action : Entry allowed. Fine notice sent.\n";
    } else {
        cout << "  [DATABASE] Status : CLEAR  ✓\n";
    }

    // =========================================================
    // STAGE 3 — AI Weight Fraud Detection
    // =========================================================
    cout << "\n  STAGE 3  ► AI Weight Fraud Detection\n";
    divider();

    if (FraudDetector::isFraud(vehicleType, weightTons)) {
        FraudDetector::printFraudAlert(vehicleType, weightTons, verifiedPlate);
        // Also alert security for weight fraud (could be overloaded axle or
        // a commercial vehicle disguised as private)
        string alertReason = "Weight Fraud: " + vehicleType + " declared but "
                           + to_string(weightTons) + "t detected";
        Notification::sendSecurityAlert(alertReason, verifiedPlate);
        return false;   // barrier stays closed
    }

    FraudDetector::printWeightOK(vehicleType, weightTons);

    // =========================================================
    // All 3 stages passed
    // =========================================================
    dividerDbl();
    cout << "  [AI PIPELINE] ALL STAGES PASSED. Barrier opening...\n";
    dividerDbl();
    return true;
}

// ============================================================================
//  SECTION 12 — MAIN FUNCTION
// ============================================================================
int main() {
    // Seed random number generator for CameraSystem tamper simulation
    srand(static_cast<unsigned int>(time(nullptr)));

    // ── Pre-registered FASTag wallets (linked to team plates) ────────────────
    vector<Wallet> wallets;
    wallets.push_back(Wallet("Lokesh",  "AP09AB1234", 500.0));
    wallets.push_back(Wallet("Mathews", "AP09CD5678", 300.0));
    wallets.push_back(Wallet("Jishnu",  "AP09EF9012", 750.0));
    wallets.push_back(Wallet("Rohit",   "AP09GH3456", 200.0));
    wallets.push_back(Wallet("Greysi",  "AP09IJ7890", 450.0));

    Database db;   // Central government registry (in-memory for this session)

    // ── Session banner ────────────────────────────────────────────────────────
    cout << "\n"
         << "  ╔══════════════════════════════════════════╗\n"
         << "  ║   TOLL TAX MANAGEMENT SYSTEM  v3.0       ║\n"
         << "  ║   STOP-FREE AI AUTOMATION EDITION        ║\n"
         << "  ║   SRM University-AP  |  OOP C++ Project  ║\n"
         << "  ╚══════════════════════════════════════════╝\n"
         << "  Team: Lokesh | Mathews | Jishnu | Rohit | Greysi\n\n";

    if (isPeakHour())
        cout << "  *** PEAK HOUR ACTIVE — Surge pricing (1.5x) in effect ***\n\n";

    int choice;
    do {
        cout << "\n  ===== MAIN MENU =====\n"
             << "  1.  Car\n"
             << "  2.  Bus\n"
             << "  3.  Truck\n"
             << "  4.  Bike\n"
             << "  5.  Ambulance\n"
             << "  ─────────────────────\n"
             << "  6.  Show Session Total\n"
             << "  7.  Show Exempt Vehicle Log\n"
             << "  8.  Save Session Data to File\n"
             << "  9.  FASTag Wallet Management\n"
             << "  10. Database Summary\n"
             << "  0.  Exit & Save\n"
             << "  ─────────────────────\n"
             << "  Choice: ";
        cin >> choice;
        divider();

        switch (choice) {

            // ── Vehicle entry cases (1-5): run full AI pipeline first ─────────
            case 1:  // Car
            case 2:  // Bus
            case 3:  // Truck
            case 4:  // Bike
            case 5: { // Ambulance
                // Determine vehicle type label for AI pipeline
                string vType;
                switch (choice) {
                    case 1: vType = "Car";       break;
                    case 2: vType = "Bus";       break;
                    case 3: vType = "Truck";     break;
                    case 4: vType = "Bike";      break;
                    case 5: vType = "Ambulance"; break;
                }

                // ── Input: plate and distance ────────────────────────────────
                string plate;
                cout << "  License Plate (e.g. AP09AB1234): ";
                cin  >> plate;

                int dist;
                cout << "  Distance (km): ";
                cin  >> dist;

                // ── Input: weight (for all types — AI fraud check uses it) ────
                //    Truck: operator always enters; others: ask for AI demo.
                //    In a real system a weight sensor provides this automatically.
                double weight = 0.0;
                switch (choice) {
                    case 1: // Car
                        cout << "  Vehicle weight in tons (sensor reading): ";
                        cin  >> weight;
                        break;
                    case 2: // Bus — auto-assign typical bus weight
                        weight = 9.0;
                        cout << "  [SENSOR] Bus weight auto-read: 9.0 tons\n";
                        break;
                    case 3: // Truck — operator must declare for penalty calc
                        cout << "  Truck weight (tons): ";
                        cin  >> weight;
                        break;
                    case 4: // Bike
                        cout << "  Vehicle weight in tons (sensor reading): ";
                        cin  >> weight;
                        break;
                    case 5: // Ambulance — standard weight
                        weight = 4.0;
                        cout << "  [SENSOR] Ambulance weight auto-read: 4.0 tons\n";
                        break;
                }

                // ── Run the 3-stage AI pipeline ──────────────────────────────
                string ownerName;
                bool   aiCleared = runAIPipeline(plate, vType, weight,
                                                  db, ownerName);

                if (!aiCleared) {
                    cout << "\n  [SYSTEM] Entry DENIED by AI pipeline."
                         << " Incident logged.\n";
                    break;   // back to main menu
                }

                // ── Post-AI inputs (pass, return journey) ────────────────────
                //    Only collected AFTER AI clearance — matches real-world flow
                PassType p = choosePass();
                int ret;
                cout << "  Return Journey? (0=No  1=Yes): ";
                cin  >> ret;

                // ── FASTag auto-lookup by plate (Feature D: auto-billing) ─────
                Wallet* w = findWalletByPlate(wallets, plate);
                if (w)
                    cout << "  [AUTO-BILLING] FASTag linked for "
                         << ownerName << " — will auto-deduct.\n";
                else {
                    cout << "  [AUTO-BILLING] No FASTag linked to this plate.\n";
                    w = chooseWallet(wallets);   // manual fallback
                }

                // ── Instantiate vehicle object + process toll ─────────────────
                //    autoBill = true so calculateToll skips any manual confirm
                switch (choice) {
                    case 1: {
                        Car v(dist, p, ret == 1, plate);
                        v.calculateToll(w, ownerName, true);
                        break;
                    }
                    case 2: {
                        Bus v(dist, p, ret == 1, plate);
                        v.calculateToll(w, ownerName, true);
                        break;
                    }
                    case 3: {
                        Truck v(dist, weight, p, ret == 1, plate);
                        v.calculateToll(w, ownerName, true);
                        break;
                    }
                    case 4: {
                        Bike v(dist, p, ret == 1, plate);
                        v.calculateToll(w, ownerName, true);
                        break;
                    }
                    case 5: {
                        Ambulance v(dist, plate);
                        v.calculateToll(nullptr, "Emergency Services", false);
                        break;
                    }
                }
                break;
            }

            // ── Non-vehicle menu options ───────────────────────────────────────
            case 6:
                Vehicle::showTotal();
                break;

            case 7:
                Vehicle::showExemptLog();
                break;

            case 8:
                Vehicle::saveCollectionToFile();
                break;

            // ── FASTag Wallet Management ───────────────────────────────────────
            case 9: {
                cout << "\n  === FASTag Wallet Manager ===\n"
                     << "  1. View all wallets\n"
                     << "  2. Register new FASTag wallet\n"
                     << "  3. Top up a wallet\n"
                     << "  Sub-choice: ";
                int sub; cin >> sub;

                if (sub == 1) {
                    cout << "\n  --- Registered FASTag Wallets ---\n";
                    for (auto& wl : wallets) wl.display();

                } else if (sub == 2) {
                    string name, vNum; double bal;
                    cout << "  Owner name       : "; cin >> name;
                    cout << "  License plate    : "; cin >> vNum;
                    cout << "  Opening balance  : Rs."; cin >> bal;
                    wallets.push_back(Wallet(name, vNum, bal));
                    // Also register plate in database so it shows as CLEAR
                    db.addRecord(VehicleRecord(vNum, name, VehicleStatus::CLEAR));
                    cout << "  FASTag & database record created for " << name << "\n";

                } else if (sub == 3) {
                    cout << "  --- Select wallet to top up ---\n";
                    for (size_t i = 0; i < wallets.size(); i++) {
                        cout << "  " << (i + 1) << ". ";
                        wallets[i].display();
                    }
                    int idx; double amt;
                    cout << "  Wallet number : "; cin >> idx;
                    cout << "  Amount (Rs.)  : "; cin >> amt;
                    if (idx >= 1 && idx <= static_cast<int>(wallets.size()))
                        wallets[idx - 1].topUp(amt);
                    else
                        cout << "  Invalid selection.\n";
                } else {
                    cout << "  Invalid sub-choice.\n";
                }
                break;
            }

            // ── Database Summary ───────────────────────────────────────────────
            case 10:
                db.printSummary();
                break;

            // ── Exit ───────────────────────────────────────────────────────────
            case 0:
                Vehicle::saveCollectionToFile();
                cout << "\n  Session data saved. Thank you! Goodbye.\n\n";
                break;

            default:
                cout << "  Invalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}

/*
 ============================================================================
  FILES GENERATED AT RUNTIME
  ─────────────────────────────────────────────────────────────────────────
  vehicle_log.txt       — timestamped entry for every vehicle processed
  daily_collection.txt  — cumulative session totals across multiple runs
  fraud_log.txt         — timestamped record of every AI fraud alert raised
  security_alerts.txt   — log of stolen-vehicle and tamper incidents

  All files use ios::app — data accumulates across sessions without overwriting.

  DEMO TIPS (for presentation)
  ─────────────────────────────────────────────────────────────────────────
  Test ALPR tamper   : re-run the same plate if pipeline fails (10 % chance)
  Test Stolen        : enter plate  TN22GH9999  or  MH12PQ5500
  Test Unpaid Fines  : enter plate  KA05MN3344  (Rs.850) or  TS08PQ7777
  Test Weight Fraud  : choose Car, enter weight 15.0 tons
  Test FASTag auto   : enter plate  AP09AB1234  (Lokesh — wallet pre-loaded)
  Test Pass waiver   : choose Daily or Monthly pass — toll is waived entirely
 ============================================================================
*/
