from typing import Tuple
import numpy as np
import sys
sys.path.append("./decision_tree/Py_Master")
import logic


class Environment_2048:
    def __init__(self, size: int = 4) -> None:
        """
        ::size:: -> row/column length 
        ::done:: -> indicates if the game is over
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

        reward = self.calculate_reward(self.state, next_state)
        self.state = next_state
        return next_state, reward, self.done, info

    def get_next_state(self, action):
        """
        This functions checks if the action is "legal".
        An action is legal when tiles merge.
        If the action is illegal, the state remains the same
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

    def calculate_reward(self, old_state, next_state):
        """
        Calculates the reward/penalty for taking a specific action
        """
        reward = 0

        # Reward for increase in score (sum of tiles)
        reward += self.calculate_score_increase(old_state, next_state)

        # Reward for tile merges
        reward += self.calculate_tile_merges(old_state, next_state)

        # Bonus reward for reaching higher tiles
        max_tile = max(max(row) for row in next_state)
        if max_tile >= 512:
            reward += max_tile // 512

        # Reward for game progression
        if not self.done:
            reward += 0.1

        # Rewards based on board evaluation
        reward += self.evaluate_board(next_state) - \
            self.evaluate_board(old_state)

        # Penalty for non-effective moves
        if np.array_equal(old_state, next_state):
            reward -= 1

        game_state = logic.game_state(next_state)
        # Penalty for losing game
        if game_state == "lose":
            reward -= 2048

        return reward

    def calculate_score_increase(self, old_state, next_state):
        """
        This functions calculates the difference in score between given states.
        The score of a state is calculated as the sum of its tiles
        """
        old_score = sum(sum(row) for row in old_state)
        next_score = sum(sum(row) for row in next_state)
        return next_score - old_score

    def calculate_tile_merges(self, old_state, next_state):
        """
        This functions iterates through the board and for each tile that has changed,
        adds the difference in value to the merges_reward
        """
        merges_reward = 0
        for i in range(self.size):
            for j in range(self.size):
                if next_state[i][j] != old_state[i][j]:
                    merges_reward += next_state[i][j] - old_state[i][j]
        return merges_reward

    def evaluate_board(self, state):
        """
        This functions evaluates a given board state, calculating a score based on 
        the values of the tiles and their positions.
        A weighted matrix weight_matrix is used to give different weights to different positions on the board.
        """
        score = 0
        weight_matrix = [[0, 1, 2, 3], [7, 6, 5, 4],
                         [8, 9, 10, 11], [15, 14, 13, 12]]
        for i in range(4):
            for j in range(4):
                if state[i][j] != 0:
                    score += np.log2(state[i][j]) * weight_matrix[i][j]
        return score

    def calculate_state_score(self):
        return np.sum(np.array(self.state))


class Action_Space_2048:
    def __init__(self, n: int = 4) -> None:
        self.n = n
        self.action_space = [x for x in range(0, n)]

    def sample(self):
        return np.random.randint(0, self.n)
