Adventure
=========

A simple configuration driven text based adevnture game engine.

Config file format
------------------

The game configuration is specified in standard json.  There are a set of expected objects that must be present in the
configuration in order to produce a working game, which specify the behavior of the parsing engine, the items in the
dungeon, as well as the dungeon itself.

Words recognized by the parser are contained within the "words object in the config.  Words are specified as arrays named
for the grammar rule they belong to.  

For example, to specify that the inventory can be accessed with both the words inventory and i:

```
"words": {
  ...
  "view_inventory": ["inventory", "i"],
  ...
}
```

The rules for parsing the format of a line are specified in the grammar object.  A key in the grammar object specifies
the type of the first word on a line, and the valueis an array of the required types of all subsequent words in the
line in the order they are expected.

If a type is specified with a * as the leading character, that word type is optional in the parsing of the line.

To specify action behavior with an optional preposition:
```
"grammar": {
  ...
  "action": ["*preposition", "item"],
  ...
}
```

The engine has built in rule sets it expects in order to trigger actions in the game, but the structure of how
those are parsed is up to the configuration.

Currently understood rules:
* introspect
* action
* add_inventory
* del_inventory
* view_inventory
* movement
* help_text
* quit

Items within the dungeon are specified using the items key, which contains item objects.  An item object must have a
description and an action.  Actions describe how the item can change the state of the room when used, and have a
description for the action.  State must be specified as a key with an integer.

Example:
```
"items": {
  ...
  "lantern": {
      "description": "An old rusty lantern.  Filled with oil, but in very poor condition.",
      "action": {
        "light": 1,
        "description": "The area is now illuminated, but the lantern won't stay lit for long."
      }
    },
  ...
}
```

The dungeon is specified with the "dungeon" key, and contains rooms.  Each room is specified with a name for the key, and multiple options within a room.  

A room must have a description, and a neighbors table saying how it connects to the rest of the dungeon.  Neighbors are specified by direction and name of room.

A room can optionally contain items, specified as an array of item names.  The entry point into the dungeon is specified with the key "start".

Example:
```
"dungeon": {
  "0:0": {
    "start": true,
    "description": "You are in a room filled with old wooden crates.  You can hear noises coming from the north.",
    "inspect": "There is an old rusty lantern nearby.  It looks like it still has oil and flint.",
    "neighbors": {
      "north": "0:1",
      "south": "0:-1",
      "east": "-1:0"
    },
    "items": ["lantern"]
  },
  ...
}
```

State contained within the room can be specified by the state key.  The inspection behavior of a room can be specified as either just a string, or dependent on the room state.  The inspect string returned by the room is selected by the the number contained within the state value, so if light is 0, string 0 will print, and so forth.

Example:
```
...
"0:1": {
  "description": "The room is pitch black.  You can hear something shuffling in the corner.",
  "inspect": {
    "light": ["You can't see anything.  The shuffling is getting closer.",
     "There is a perfectly average looking villager standing alone in the room.  He stares at you blankly."]
  },
  "neighbors": {
    "south": "0:0"
  },
  "state": {
    "light": 0
  }
},
...
```
