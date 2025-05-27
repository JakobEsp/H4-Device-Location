#pragma once

class Unix {
    public:
    static unsigned long getTimestamp() {
        return timestamp;
    }
    static void setTimestamp(unsigned long ts) {
        timestamp = ts;
    }
    private:

    static unsigned long timestamp;
    
};