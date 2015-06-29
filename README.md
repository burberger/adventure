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

