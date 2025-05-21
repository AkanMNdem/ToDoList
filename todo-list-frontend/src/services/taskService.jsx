import axios from 'axios';

const API_URL = 'http://localhost:8080/api';

export const getTasks = async () => {
  const response = await axios.get(`${API_URL}/tasks`);
  return response.data;
};

export const getTask = async (id) => {
  const response = await axios.get(`${API_URL}/tasks/${id}`);
  return response.data;
};

export const createTask = async (task) => {
  const response = await axios.post(`${API_URL}/tasks`, task);
  return response.data;
};

export const updateTask = async (id, task) => {
  const response = await axios.put(`${API_URL}/tasks/${id}`, task);
  return response.data;
};

export const deleteTask = async (id) => {
  await axios.delete(`${API_URL}/tasks/${id}`);
};

export const completeTask = async (id) => {
  await axios.post(`${API_URL}/tasks/${id}/complete`);
};

export const uncompleteTask = async (id) => {
  await axios.post(`${API_URL}/tasks/${id}/uncomplete`);
};

export const getPrioritizedTasks = async () => {
  const response = await axios.get(`${API_URL}/tasks/prioritized`);
  return response.data;
};

export const getCompletedTasks = async () => {
  const response = await axios.get(`${API_URL}/tasks/completed`);
  return response.data;
};
