import React, { useState } from 'react';
import { createTask } from '../services/taskService';

const TaskForm = ({ onTaskAdded }) => {
  const [taskData, setTaskData] = useState({
    header: '',
    description: '',
    difficulty: 3,
    dueDate: ''
  });

  const handleChange = (e) => {
    const { name, value } = e.target;
    setTaskData({
      ...taskData,
      [name]: value
    });
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    try {
      await createTask(taskData);
      setTaskData({
        header: '',
        description: '',
        difficulty: 3,
        dueDate: ''
      });
      onTaskAdded();
    } catch (error) {
      console.error('Error creating task:', error);
    }
  };

  return (
    <div style={{ marginBottom: '20px', padding: '20px', border: '1px solid #ddd', borderRadius: '4px' }}>
      <h2>Add New Task</h2>
      <form onSubmit={handleSubmit}>
        <div style={{ marginBottom: '10px' }}>
          <label htmlFor="header" style={{ display: 'block', marginBottom: '5px' }}>Title:</label>
          <input
            type="text"
            id="header"
            name="header"
            value={taskData.header}
            onChange={handleChange}
            required
            style={{ width: '100%', padding: '8px', borderRadius: '4px', border: '1px solid #ddd' }}
          />
        </div>

        <div style={{ marginBottom: '10px' }}>
          <label htmlFor="description" style={{ display: 'block', marginBottom: '5px' }}>Description:</label>
          <textarea
            id="description"
            name="description"
            value={taskData.description}
            onChange={handleChange}
            style={{ width: '100%', padding: '8px', borderRadius: '4px', border: '1px solid #ddd', minHeight: '100px' }}
          />
        </div>

        <div style={{ marginBottom: '10px' }}>
          <label htmlFor="difficulty" style={{ display: 'block', marginBottom: '5px' }}>Difficulty (1-5):</label>
          <input
            type="number"
            id="difficulty"
            name="difficulty"
            min="1"
            max="5"
            value={taskData.difficulty}
            onChange={handleChange}
            required
            style={{ width: '100%', padding: '8px', borderRadius: '4px', border: '1px solid #ddd' }}
          />
        </div>

        <div style={{ marginBottom: '10px' }}>
          <label htmlFor="dueDate" style={{ display: 'block', marginBottom: '5px' }}>Due Date:</label>
          <input
            type="date"
            id="dueDate"
            name="dueDate"
            value={taskData.dueDate}
            onChange={handleChange}
            style={{ width: '100%', padding: '8px', borderRadius: '4px', border: '1px solid #ddd' }}
          />
        </div>

        <button 
          type="submit"
          style={{
            backgroundColor: '#007bff',
            color: 'white',
            border: 'none',
            padding: '10px 15px',
            borderRadius: '4px',
            cursor: 'pointer'
          }}
        >
          Add Task
        </button>
      </form>
    </div>
  );
};

export default TaskForm;