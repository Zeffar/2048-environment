from typing import Tuple
import numpy as np
import sys
sys.path.append("./decision_tree/Py_Master")

import logic

class Environment_2048:
    def __init__(self, size : int = 4) -> None:
        self.size = size
        self.done = False
        self.state = logic.new_game(size)
        self.action_space = Action_Space_2048(size)
        self.observation_space = Observation_Space_2048(size)

    def reset(self):
        self.state = logic.new_game(self.size)
        self.done = False
        return self.state

    def step(self, action : int) -> Tuple[np.ndarray, int, bool, str]:
        commands = {
            0: logic.up,
            1: logic.right,
            2: logic.down,
            3: logic.left,
        }
        """
        ::next_state:: -> n x n matrix containing the state resulted from using action
        ::action:: -> integer from [0,1,2,3] representing which way we slide the tiles
        ::self.done:: -> indicates if the game is over
        ::info:: -> tells us if the game is won/lost, empty string if terminated = False
        """
        info = ""
        next_state, _ = commands[action](self.state)
        game_state = logic.game_state(next_state)
        
        if game_state != "not over":
            info = game_state
            self.done = True
        
        self.state = next_state
        reward = self.calculate_reward()
        return next_state, reward, self.done, info

    def render(self):
        pass

    def calculate_reward(self):
        pass

class Action_Space_2048:
    def __init__(self, n : int = 4) -> None:
        self.n = n
        self.action_space = [x for x in range(0, n)]
    
    def sample(self):
        return np.random.randint(0,4)

class Observation_Space_2048:
    def __init__(self, n : int = 4) -> None:
        self.n = n ** 2
