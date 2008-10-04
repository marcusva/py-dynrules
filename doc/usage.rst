Getting started
===============

dynrules uses a simple, but flexible class layout to configure rulesets
and create scripts from them. In order to use the dynrules package,
simply import it using ::

    import dynrules

You now can start to create own rulesets and set up your script
generator.

Creating rules
==============

A rule defines certain criteria of an object's behaviour. The ``Rule``
class features the most basic needs to set up your own rules for
scripts. It consists of an *id*, a *weight* and *code* that defines the
rule's action.

Let's imagine, you have a Warrior class that can walk in four
directions and fight against enemies. Defining those actions might look
like ::

  class Warrior:
      def do_walk (direction):
          ...
      def fight (enemy):
          ...
      ...

Automating walking and fighting requires a lot of work regarding the
priorities of when to walk and when to fight. Fighting a stronger enemy
might be senseless, so the Warrior should walk away from it. Let's try
to formulate some rules for it: ::

  if is_enemy_at (direction):
      if warrior.strength > enemy.strength:
          warrior.fight (enemy)
      else:
          warrior.do_walk (~direction)
  else:
      warrior.do_walk (direction)

What does the above code do? First it checks, whether there is an enemy
at the specified direction. If it is, and it is weaker than the warrior,
the warrior will fight it. Otherwise, the warrior will go into the
opposite direction to escape a possible fight. At last, if no enemy is
found at the given direction, the warrior will walk towards it.

This is a predictable, typical behaviour and sometimes it might happen
that, although the warrior is weaker, it will fight the enemy with
success. That means, for a more unpredictable behaviour, we have four
possible actions:

#. warrior.strength > enemy.strength: fight enemy
#. warrior.strength < enemy.strength: flee from enemy
#. warrior.strength < enemy.strength: fight enemy
#. warrior.strength > enemy.strength: flee from enemy

The first and second rule are directly from the solution above. They
make the most sense in those cases and should be preferred. Rule three
and four shall offer some more unpredictable behaviour.

Let's formulate some rules for the enemy detection scenario using
dynrules. ::

    # Create a new Rule for fighting the enemy.
    rule1 = new Rule (1)
    rule1.weight = 10
    rule1.code = "if warrior.strength > enemy.strength: warrior.fight (enemy)"

    # Create another Rule for fleeing.
    rule2 = new Rule (2)
    rule2.weight = 10
    rule2.code = "if warrior.strength < enemy.strength: warrior.do_walk (~direction)"

    # Fighting a stronger enemy
    rule3 = new Rule (3)
    rule3.weight = 5
    rule3.code = "if warrior.strength < enemy.strength: warrior.fight (enemy)"

    # Fleeing from a weaker enemy
    rule4 = new Rule (4)
    rule4.weight = 5
    rule4.code = "if warrior.strength > enemy.strength: warrior.do_walk (~direction)"

We set up the necessary rules. Now it's time to put them together in a
``RuleSet`` that takes care of them.

Weighting rules - the RuleSet
=============================

The RuleSet takes care of updating the weight of the ``Rule`` objects it
contains.
