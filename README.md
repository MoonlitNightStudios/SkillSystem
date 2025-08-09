# SkillSystem
 A plugin that provides a skill system in Unreal Engine.

# What is it?
### The skill system is a plugin for [Unreal engine 5](https://www.unrealengine.com/en-US/) that provides tools to design and use skill trees in Unreal Engine. It provides a graph editor to create skill trees and during runtime can generate widgets for the skill tree.

The skill system plugin is being used in our projects and any improvements we make to the plugin will reflect here.

## Getting Started
1. Add the skill system plugin to your project's plugin folder and enable it.
1. Right click on the content browser and under the skill system category select skill tree.
1. Skill graphs have a default root node. We plan on adding support to adding root nodes since some games could use skill trees with multiple root nodes. Right click to add a **Skill** node.
1. Set the skill node widget class in details panel. This widget class needs to implement ISkillNodeWidgetInterface. Each skill node allows overriding this widget class.
1. Set the widget class for the root node.
1. Set the connection drawing policy. The plugin provides a default policy that just draws lines between connected nodes. We intend to add more.

# Highlights
### Graph editor
The graph editor makes it easy to design skill trees.

![widget](https://github.com/MoonlitNightStudios/SkillSystem/blob/release/Docs/Screenshot_20250809_011418.png)

### Generate widget
Generate a widget at runtime. Each skill icon is clickable and developers can listen to an OnClick event.

![widget](https://github.com/MoonlitNightStudios/SkillSystem/blob/release/Docs/Screenshot_20250809_012125.png)
