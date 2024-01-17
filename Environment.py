import logic
from typing import Tuple
import numpy as np
import sys
sys.path.append("./decision_tree/Py_Master")


class Environment_2048:
    def __init__(self, size: int = 4) -> None:
        """
        ::size:: -> row/column length 
        ::done:: -> inicates if the game is over
        ::state:: -> n x n matrix containing the current state of the game
        ::action_space :: -> class object that contains the possible actions
        """

        self.size = size
        self.done = False
        self.state = logic.new_game(size)
        self.action_space = Action_Space_2048(size)

    def reset(self):
        """
        Resets the environment to a random starting state
        """

        self.state = logic.new_game(self.size)
        self.done = False
        return self.state

    def step(self, action: int) -> Tuple[np.ndarray, int, bool, str]:
        """
        ::next_state:: -> n x n matrix containing the state resulted from using action
        ::action:: -> integer from [0,1,2,3] representing which way we slide the tiles
        ::self.done:: -> indicates if the game is over
        ::info:: -> tells us if the game is won/lost, empty string if terminated = False
        """
        info = ""
        next_state = self.get_next_state(action)
        game_state = logic.game_state(next_state)

        if game_state != "not over":
            info = game_state
            self.done = True

        reward = self.calculate_reward(next_state)
        self.state = next_state
        return next_state, reward, self.done, info

    def get_next_state(self, action):
        """
        This functions checks if the action is "legal".
        An action is legal when tiles merge 
        If the action is ilegal, the state remains the same

        """
        commands = {
            0: logic.up,
            1: logic.right,
            2: logic.down,
            3: logic.left,
        }

        state, legal = commands[action](self.state)

        if legal:
            state = logic.add_two(state)

        return state

    def render(self):
        # TODO -> optional
        pass

    def calculate_reward(self, next_state):
        # TODO -> mandatory
        pass


class Action_Space_2048:
    def __init__(self, n: int = 4) -> None:
        self.n = n
        self.action_space = [x for x in range(0, n)]

    def sample(self):
        return np.random.randint(0, self.n)
