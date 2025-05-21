import axios from 'axios';

// Use relative URL to work with proxy configuration
const API_URL = '/api';

// Add request interceptor for debugging with more detailed logging
axios.interceptors.request.use(request => {
  console.log('Request Config:', {
    url: request.url,
    method: request.method,
    headers: request.headers,
    data: request.data
  });
  return request;
}, error => {
  console.error('Request Error:', error);
  return Promise.reject(error);
});

// Add response interceptor for debugging with more detailed error handling
axios.interceptors.response.use(response => {
  console.log('Response Status:', response.status);
  console.log('Response Data:', response.data);
  return response;
}, error => {
  if (error.response) {
    // The request was made and the server responded with a status code
    // that falls out of the range of 2xx
    console.error('Response Error Data:', error.response.data);
    console.error('Response Error Status:', error.response.status);
    console.error('Response Error Headers:', error.response.headers);
  } else if (error.request) {
    // The request was made but no response was received
    console.error('No response received:', error.request);
  } else {
    // Something happened in setting up the request that triggered an Error
    console.error('Request Setup Error:', error.message);
  }
  console.error('Error Config:', error.config);
  return Promise.reject(error);
});

export const getTasks = async () => {
  try {
    const response = await axios.get(`${API_URL}/tasks`);
    return response.data;
  } catch (error) {
    console.error('Error in getTasks:', error);
    throw error;
  }
};

export const getTask = async (id) => {
  try {
    const response = await axios.get(`${API_URL}/tasks/${id}`);
    return response.data;
  } catch (error) {
    console.error('Error in getTask:', error);
    throw error;
  }
};

export const createTask = async (task) => {
  try {
    const response = await axios.post(`${API_URL}/tasks`, task);
    return response.data;
  } catch (error) {
    console.error('Error in createTask:', error);
    console.error('Task data:', JSON.stringify(task));
    throw error;
  }
};

export const updateTask = async (id, task) => {
  try {
    const response = await axios.put(`${API_URL}/tasks/${id}`, task);
    return response.data;
  } catch (error) {
    console.error('Error in updateTask:', error);
    throw error;
  }
};

export const deleteTask = async (id) => {
  try {
    await axios.delete(`${API_URL}/tasks/${id}`);
  } catch (error) {
    console.error('Error in deleteTask:', error);
    throw error;
  }
};

export const completeTask = async (id) => {
  try {
    await axios.post(`${API_URL}/tasks/${id}/complete`);
  } catch (error) {
    console.error('Error in completeTask:', error);
    throw error;
  }
};

export const uncompleteTask = async (id) => {
  try {
    await axios.post(`${API_URL}/tasks/${id}/uncomplete`);
  } catch (error) {
    console.error('Error in uncompleteTask:', error);
    throw error;
  }
};

export const getPrioritizedTasks = async () => {
  try {
    const response = await axios.get(`${API_URL}/tasks/prioritized`);
    return response.data;
  } catch (error) {
    console.error('Error in getPrioritizedTasks:', error);
    throw error;
  }
};

export const getCompletedTasks = async () => {
  try {
    const response = await axios.get(`${API_URL}/tasks/completed`);
    return response.data;
  } catch (error) {
    console.error('Error in getCompletedTasks:', error);
    throw error;
  }
};
