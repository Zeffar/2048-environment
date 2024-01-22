import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np
from DQL_Networks import DQN, ReplayBuffer


class DQNAgent:
    def __init__(self, state_size, action_size, action_space):
        self.state_size = state_size
        self.action_space = action_space
        self.action_size = action_size
        self.memory = ReplayBuffer(10000)
        self.model = DQN(state_size, action_size)
        self.target_model = DQN(state_size, action_size)
        self.target_model.load_state_dict(self.model.state_dict())
        self.optimizer = optim.Adam(self.model.parameters(), lr=1e-4)
        self.loss_fn = nn.MSELoss()

        # HYPERPARAMETERS

        # Discount factor
        self.gamma = 0.995

        # Exploration rate
        self.epsilon = 1.0
        self.epsilon_min = 0.1
        self.epsilon_decay = 0.0001

    def choose_action(self, state):
        if np.random.rand() <= self.epsilon:
            return np.random.choice(self.action_space)
        else:
            state = torch.from_numpy(state).float().unsqueeze(0)
            with torch.no_grad():
                action_values = self.model(state)
            return np.argmax(action_values.cpu().data.numpy())

    def learn(self, batch_size):
        if len(self.memory) < batch_size:
            return

        minibatch = self.memory.sample(batch_size)
        states, actions, rewards, next_states, dones = zip(*minibatch)

        states = torch.tensor(np.array(states), dtype=torch.float32)
        actions = torch.tensor(np.array(actions), dtype=torch.long)
        rewards = torch.tensor(np.array(rewards), dtype=torch.float32)
        next_states = torch.tensor(np.array(next_states), dtype=torch.float32)

        Q_expected, _ = torch.max(self.model(states).squeeze(1), dim=1)
        Q_targets_next, _ = torch.max(
            self.target_model(next_states).squeeze(1), dim=1)
        Q_targets = rewards + self.gamma * Q_targets_next

        loss = self.loss_fn(Q_expected, Q_targets)
        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()

    def update_target_network(self):
        self.target_model.load_state_dict(self.model.state_dict())
