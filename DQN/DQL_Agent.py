import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np
import random 
from DQN_networks import DQN, ReplayBuffer

class DQNAgent:
    def __init__(self, state_size, action_size):
        self.state_size = state_size
        self.action_size = action_size
        self.memory = ReplayBuffer(10000)
        self.model = DQN(state_size, action_size)
        self.target_model = DQN(state_size, action_size)
        self.target_model.load_state_dict(self.model.state_dict())
        self.optimizer = optim.Adam(self.model.parameters(), lr=1e-4)
        self.loss_fn = nn.MSELoss()

        # Hyperparameters
        self.gamma = 0.99  # Discount factor
        self.epsilon = 1.0  # Exploration rate
        self.epsilon_min = 0.01
        self.epsilon_decay = 0.995

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

        states = torch.tensor(states, dtype=torch.float32)
        actions = torch.tensor(actions, dtype=torch.long)
        rewards = torch.tensor(rewards, dtype=torch.float32)
        next_states = torch.tensor(next_states, dtype=torch.float32)
        dones = torch.tensor(dones, dtype=torch.float32)

        Q_expected = self.model(states).gather(1, actions.unsqueeze(1)).squeeze(1)
        Q_targets_next = self.target_model(next_states).max(1)[0].detach()
        Q_targets = rewards + (self.gamma * Q_targets_next * (1 - dones))

        loss = self.loss_fn(Q_expected, Q_targets)
        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()

        if self.epsilon > self.epsilon_min:
            self.epsilon *= self.epsilon_decay

    def update_target_network(self):
        self.target_model.load_state_dict(self.model.state_dict())