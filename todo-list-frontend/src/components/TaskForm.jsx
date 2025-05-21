import React, { useState } from 'react';
import { createTask } from '../services/taskService';

const TaskForm = ({ onTaskAdded }) => {
  const [task, setTask] = useState({
    header: '',
    description: '',
    difficulty: 3,
    dueDate: ''
  });

  const [error, setError] = useState('');

  const handleChange = (e) => {
    const { name, value } = e.target;
    setTask({
      ...task,
      [name]: name === 'difficulty' ? parseInt(value, 10) : value
    });
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    
    if (!task.header) {
      setError('Task title is required');
      return;
    }

    if (task.difficulty < 1 || task.difficulty > 5) {
      setError('Difficulty must be between 1 and 5');
      return;
    }

    try {
      await createTask(task);
      setTask({
        header: '',
        description: '',
        difficulty: 3,
        dueDate: ''
      });
      setError('');
      onTaskAdded();
    } catch (error) {
      console.error('Error creating task:', error);
      setError('Failed to create task');
    }
  };

  return (
    <div className="task-form" style={{ 
      marginBottom: '30px',
      padding: '20px',
      borderRadius: '8px', 
      backgroundColor: 'var(--card-bg)',
      boxShadow: 'var(--shadow)',
      border: '1px solid var(--border-color)'
    }}>
      <h2 style={{ 
        marginTop: '0',
        color: 'var(--text-color)',
        textAlign: 'center'
      }}>Add New Task</h2>
      
      {error && (
        <div style={{ 
          padding: '10px', 
          marginBottom: '15px', 
          color: 'white', 
          backgroundColor: 'var(--danger-color)',
          borderRadius: '4px'
        }}>
          {error}
        </div>
      )}

      <form onSubmit={handleSubmit}>
        <div style={{ marginBottom: '15px' }}>
          <label 
            htmlFor="header"
            style={{
              display: 'block',
              marginBottom: '5px',
              color: 'var(--text-color)',
              fontWeight: '500'
            }}
          >
            Task Title *
          </label>
          <input
            type="text"
            id="header"
            name="header"
            value={task.header}
            onChange={handleChange}
            style={{
              width: '100%',
              padding: '10px',
              boxSizing: 'border-box',
              backgroundColor: 'var(--bg-primary)',
              color: 'var(--text-color)',
              border: '1px solid var(--border-color)',
              borderRadius: '4px'
            }}
          />
        </div>

        <div style={{ marginBottom: '15px' }}>
          <label 
            htmlFor="description"
            style={{
              display: 'block',
              marginBottom: '5px',
              color: 'var(--text-color)',
              fontWeight: '500'
            }}
          >
            Description
          </label>
          <textarea
            id="description"
            name="description"
            value={task.description}
            onChange={handleChange}
            rows="3"
            style={{
              width: '100%',
              padding: '10px',
              boxSizing: 'border-box',
              backgroundColor: 'var(--bg-primary)',
              color: 'var(--text-color)',
              border: '1px solid var(--border-color)',
              borderRadius: '4px',
              resize: 'vertical'
            }}
          />
        </div>

        <div style={{ 
          display: 'flex',
          gap: '15px',
          marginBottom: '15px'
        }}>
          <div style={{ flex: 1 }}>
            <label 
              htmlFor="difficulty"
              style={{
                display: 'block',
                marginBottom: '5px',
                color: 'var(--text-color)',
                fontWeight: '500'
              }}
            >
              Difficulty (1-5)
            </label>
            <input
              type="number"
              id="difficulty"
              name="difficulty"
              min="1"
              max="5"
              value={task.difficulty}
              onChange={handleChange}
              style={{
                width: '100%',
                padding: '10px',
                boxSizing: 'border-box',
                backgroundColor: 'var(--bg-primary)',
                color: 'var(--text-color)',
                border: '1px solid var(--border-color)',
                borderRadius: '4px'
              }}
            />
          </div>

          <div style={{ flex: 1 }}>
            <label 
              htmlFor="dueDate"
              style={{
                display: 'block',
                marginBottom: '5px',
                color: 'var(--text-color)',
                fontWeight: '500'
              }}
            >
              Due Date
            </label>
            <input
              type="date"
              id="dueDate"
              name="dueDate"
              value={task.dueDate}
              onChange={handleChange}
              style={{
                width: '100%',
                padding: '10px',
                boxSizing: 'border-box',
                backgroundColor: 'var(--bg-primary)',
                color: 'var(--text-color)',
                border: '1px solid var(--border-color)',
                borderRadius: '4px'
              }}
            />
          </div>
        </div>

        <div style={{ textAlign: 'center' }}>
          <button 
            type="submit"
            style={{
              backgroundColor: 'var(--primary-color)',
              color: 'white',
              padding: '10px 20px',
              border: 'none',
              borderRadius: '4px',
              cursor: 'pointer',
              fontSize: '16px'
            }}
          >
            Add Task
          </button>
        </div>
      </form>
    </div>
  );
};

export default TaskForm;