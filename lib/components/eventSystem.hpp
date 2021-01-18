#pragma once
#include "lib/object.h"
#include <functional>
#include <map>
using namespace std;

struct EventData {

};

typedef void (*listener_t)(const EventData*);

class EventSystem : public Component {
  // Event listeners - e.g. `Player.attacked()`, `Tree.spawned()`
  map<string, vector<listener_t>> listeners;
 public:
  // Emit an event
  void emit(string eventName, const EventData* data) {
    for(const auto i : listeners[eventName]) (*i)(data);
  }

  // Add an listener
  void listen(string eventName, listener_t listener) {
    listeners[eventName].push_back(listener);
  }

  // Remove listener
  // Returns false if the listener was not found
  bool unlisten(string eventName, listener_t listener) {
    auto &coll = listeners[eventName];
    auto ptr = find(coll.begin(), coll.end(), listener);
    if(ptr == coll.end()) return false;
    coll.erase(ptr);
    return true;
  }
};