#pragma once
#include "object.h"
#include <functional>
#include <map>
using namespace std;

class EventSystem : public Component {
  // Event listeners - e.g. `Player.attacked()`, `Tree.spawned()`
  map<string, vector<function<void(const EventData*)>>> listeners;
 public:
  // Emit an event
  void emit(string eventName, const EventData* data) {
    for(const auto &i : listeners[eventName]) i(data);
  }

  // Add an listener
  void listen(string eventName, function<void(const EventData*)> &listener) {
    listeners[eventName].push_back(listener);
  }

  // Remove listener
  // Returns false if the listener was not found
  bool unlisten(string eventName, function<void(const EventData*)> &listener) {
    auto &coll = listeners[eventName];
    auto ptr = find(coll.begin(), coll.end(), listener);
    if(ptr == coll.end()) return false;
    coll.erase(ptr);
    return true;
  }
};