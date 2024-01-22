import numpy as np
from DQL_Agent import DQNAgent
import sys
from Environment import Environment_2048


def train_dqn(agent, env, episodes, batch_size, max_steps):
    for e in range(1, episodes + 1):
        state = env.reset()
        # flatten the state matrix
        state = np.reshape(state, [1, agent.state_size])
        for time in range(max_steps):
            action = agent.choose_action(state)
            next_state, reward, done, _ = env.step(action)
            next_state = np.reshape(next_state, [1, agent.state_size])
            agent.memory.add(state, action, reward, next_state, done)
            state = next_state

            if done:
                break

            if len(agent.memory) > batch_size:
                agent.learn(batch_size)

        if agent.epsilon * (1 - agent.epsilon_decay) ** e > agent.epsilon_min:
            agent.epsilon = agent.epsilon * (1 - agent.epsilon_decay) ** e

        # Update target model every 20 episodes
        if e % 5 == 0:
            agent.update_target_network()

        if e % 50 == 0:
            print(f"episode: {e}/{episodes}, truncated : {time == max_steps - 1}, score : {env.calculate_state_score()}, max_value : {np.max(np.array(next_state))} e: {agent.epsilon:.2}")



# create Environment
env = Environment_2048()

# number of cells on the board
state_size = env.size ** 2

# number of actions that can be taken i.e up,down,left,right
action_size = env.action_space.n

# actions that can be taken : up, down. left, right -> encoded into -> [0,1,2,3]
action_space = env.action_space.action_space

# initialize the agent
agent = DQNAgent(state_size, action_size, action_space)

train_dqn(agent, env, 1000, 64, 500)
