Getting started
===============

dynrules uses a simple, but flexible class layout to configure rulesets
and create scripts from them. In order to use the dynrules package,
simply import it using ::

    import dynrules

You now can start to create own rulesets and set up your script
generator.

Creating rules - Rule
=====================

A rule defines certain criteria of an object's behaviour. The ``Rule``
class features the most basic needs to set up your own rules for
scripts. It consists of an *id*, a *weight* and *code* that defines the
rule's action.

Let's imagine, you have a game with a Warrior class that can walk in four
directions and fight against enemies. Defining those actions might look
like ::

  class Warrior:
      def do_walk (self, direction):
          ...
      def fight (self, enemy):
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
at the specified direction the warrior should walk to. If it is, and it is 
weaker than the warrior, the warrior will fight it. Otherwise, the warrior
will go into the opposite direction to escape a possible fight. At last, if
no enemy is found at the given direction, the warrior will walk towards it.

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
  rule1 = Rule (1)
  rule1.weight = 10
  rule1.code = "if warrior.strength > enemy.strength: warrior.fight (enemy)"
  
  # Create another Rule for fleeing.
  rule2 = Rule (2)
  rule2.weight = 10
  rule2.code = "if warrior.strength < enemy.strength: warrior.do_walk (~direction)"
  
  # Fighting a stronger enemy
  rule3 = Rule (3)
  rule3.weight = 5
  rule3.code = "if warrior.strength < enemy.strength: warrior.fight (enemy)"
  
  # Fleeing from a weaker enemy
  rule4 = Rule (4)
  rule4.weight = 5
  rule4.code = "if warrior.strength > enemy.strength: warrior.do_walk (~direction)"

We set up the necessary rules. Now it's time to put them together in a
``RuleSet`` that takes care of them.

Weighting rules - RuleSet
=========================

Weighting a rule means to mark and measure its priority or importance
within a set of applicable rules. The more important a rule is, the
higher its weight should be. Measuring the importance of a rule is
usually done by counting how often it is called. The result (successful
or unsuccessful) often influences the rule's weight, but does not need
to.

The ``RuleSet`` takes care of mesuring the importance and updating the
weight of the ``Rule`` objects it contains. A ``RuleSet`` usually
consists of applicable rules for a specific situation and lets you
define methods for measuring and detecting the success of the rules. As
the process of measuring the success solely depends on the surrounding
system, the implemenation of that process can vary and the ``RuleSet``
class requires you to take care of it. ::

  class MyOwnRuleSet (RuleSet):
      def calculate_adjustment (self, fitness):
          # Implement your own success detection here.
          pass
      
      def distribute_remainder (self, remainder):
          # Implement your own remainder method here.
          pass

``calculate_adjustment(self, fitness)`` calculates the reward or
penalty, each used rule receives. The *fitness* argument can be used to
provide additional information, e.g. about the performance of the
execution.

``distribute_remainder (self, remainder)`` is called to distribute the
difference between the total weight before and after the update once the
weight updating within the ``RuleSet`` is done. This might be necessary to
allow a balancing of rule weights so that the total sum of all rules within a
``RuleSet`` will remain the same, for example. In reality however, such a
distribution solely depends on the specific application needs.

The weight update process of the ``RuleSet`` looks like ::

  def update_weights (self, fitness):
      # Initialise needed things.
      adjustment = self.calculate_adjustment (fitness)
      # Update rule weights with the adjustment and calculuate remainder
      self.distribute_remainder (remander)
      # Update new total weight.
      # return

To get a better idea about this, let's create a small ``RuleSet``
implementation for our previously created rules. We assume that the
*fitness* we receive expresses the difference between the damage the
warrior made and received during the execution of the rules. ::

  class WarriorRuleSet (RuleSet):
      def calculate_adjustment (self, fitness):
          #
          # fitness = damage_warrior_caused - damage_warrior_received
          #
          # 1) a high fitness means, the warrior caused more damage
          # 2) a very low or negative fitness means, the warrior did not
          # cause that much damage or even received more than it caused.
          #
          # for case 1) we assume the execution of the rules to be
          # successful, for case 2) we do not.
          
          # We set the sucess/fail threshold to 3.
          if fitness > 3:
              # The execution was successful, the warrior is strong!
              # The adjustment will be the total fitness - threshold.
              return fitness - 3
          else:
              # The execution was not successful, the warrior is weak!
              if fitness < 0:
                  # Lousy, simply return the negative fitness
                  return fitness 
              else:
                  # Not so lousy, return a penalty value as difference
                  # of threshold minus fitness.
                  return - (3 - fitness)

      def distribute_remainder (self, remainder):
          #
          # Here we distribute the difference of the last total weights
          # and newly calculated total weights.
          # Give each rule the same fraction.
          #
          count = len (self.rules)
          if count == 0:
              return # Safety net, if no rules are there.

          fraction = remainder / float (count)
          for rule in self.rules:
              rule.weight += fraction

Now we can add the created rules from above. ::

  warriorruleset = WarriorRuleSet (0, 20)
  warriorruleset.add (rule1)
  warriorruleset.add (rule2)
  warriorruleset.add (rule3)
  warriorruleset.add (rule4)

The both arguments of the constructor, *minweight* and *maxweight* are
the boundary limits for rules contained in a ``RuleSet``. They define
the upper and lower weight limit, each rule can have.

From now on, the ``WarriorRuleSet`` is fully functional and can update
rule weights as necessary. 

To add another level of automation and to create scripts from the rules, a
``LearnSystem`` will be necessary however.

Generating scripts - LearnSystem
================================

The ``LearnSystem`` class is used to create scripts automatically from
an existing ``RuleSet``. It generates the scripts in a programming
language neutral manner which means, that it only uses the *code*
attribute of ``Rule`` objects for creating the output.

Additionally the ``LearnSystem`` can add code to be executed before and
after the rules are entered to make the generated script fully
functional for the specific task and environment. The creation of a
script thus consists of the following tasks.

#. Create script header
#. Select rules and create code
#. Create script footer

To create a ``LearnSystem`` for the ``WarriorRuleSet``, only a single
line of code is necessary. ::

  warriorlearnsystem = LearnSystem (warriorruleset)

The ``LearnSystem`` is now full functional and you can start generating
scripts for the warrior. ::

  warriorlearnsystem.create_script ("scriptfile.scr", 4)

``create_script`` will create a new script, insert a header, add rules
to it and then add the footer. You can specify the maximum amount of
rules to be added by setting the second argument to the required value. ::

  # Add a maximum of 10 rules.
  warriorlearnsystem.create_script ("scriptfile.scr", 10)
  # Add a maximum of 3 rules.
  warriorlearnsystem.create_script ("scriptfile2.scr", 3)

You can modify several attributes and methods of the ``LearnSystem`` to
tweak it to your personal needs.

``create_header()`` and ``create_footer()`` are used to create necesary
code to add before and after the rules. That can be initialisation and
finalisation code, checks or whatever is necessary for the target
system. Both methods return a string containing the code to add. ::

  class OwnLearnSystem (LearnSystem):
      def create_header (self):
          # Create header code
          return 'def execute_rules (object):\n' + \
                 '    selected_rule = None\n'
          
      def create_footer (self):
          # Create footer code
          return '    return selected_rule\n'

The above class would generate the following code: ::

  def execute_rules (object):
      selected_rule = None
      #
      # RULE CODE
      #
      return selected_rule

The *maxscriptsize* attribute allows you to define the maximum size in
bytes of a script to generate. *maxscriptsize* does not take the header
and footer into account, but only the code generated from the rules. ::

  # Limit the size of the code generated from the rules to 4 kB.
  warriorlearnsystem.maxscriptsize = 4096

*maxtries* limits the rule selection process, so that it does not take
infinite trials to find a rule to add. This can be very helpful to
limit the time spent on selecting rules. ::

  # Only try to find new rules 50 times.
  warriorlearnsystem.maxtries = 5

