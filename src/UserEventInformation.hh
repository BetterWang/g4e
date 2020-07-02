#ifndef G4E_USEREVENTINFORMATION_HH
#define G4E_USEREVENTINFORMATION_HH

#include <G4VUserEventInformation.hh>

#include <cstdint>

namespace g4e {

    struct DisInfo {
        double   Q2;
        double   x ;
        double   y ;
        double   w2;
        double   nu;
        double   tHat;
    };

    struct EventData {
        /// Place in file where this events starts. (makes sence for text file formats only)
        uint64_t StartLine;

        /// Deep Inelastic Scattering information
        DisInfo TrueDISInfo;

        /// Flag that this event has True DIS information
        bool HasTrueDISInfo;

        /// MC event weight
        double Weight;
    };


    class UserEventInformation: public G4VUserEventInformation
    {
    public:
        void Print() const override;

        /// Place in file where this events starts. (makes sence for text file formats only)
        uint64_t StartLine() { return GetEventData().StartLine; }

        /// Deep Inelastic Scattering information
        DisInfo& GetTrueDisInfo() { return GetEventData().TrueDISInfo; }

        /// Flag that this event has True DIS information
        bool HasTrueDisInfo() { return GetEventData().HasTrueDISInfo; }

        /// MC event weight
        double Weight() { return GetEventData().Weight; }

        /// Gets g4e::EventInfo structure with the additional event information
        EventData &GetEventData() { return mEventInfo; }
        const EventData &GetEventInfoConst() const { return mEventInfo; }

        void SetEventInfo(const EventData &mEventInfo) { UserEventInformation::mEventInfo = mEventInfo; }

    private:
        EventData mEventInfo;

    };
}

#endif //G4E_USEREVENTINFORMATION_HH
