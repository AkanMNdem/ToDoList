import React from 'react';
import { completeTask, uncompleteTask, deleteTask } from '../services/taskService';

const TaskItem = ({ task, onTaskUpdated }) => {
  const handleComplete = async () => {
    try {
      if (task.completed) {
        await uncompleteTask(task.id);
      } else {
        await completeTask(task.id);
      }
      onTaskUpdated();
    } catch (error) {
      console.error('Error updating task:', error);
    }
  };

  const handleDelete = async () => {
    try {
      await deleteTask(task.id);
      onTaskUpdated();
    } catch (error) {
      console.error('Error deleting task:', error);
    }
  };

  return (
    <div className="task-item" style={{ 
      padding: '15px', 
      margin: '10px 0', 
      borderRadius: '8px',
      border: '1px solid var(--border-color)',
      backgroundColor: task.completed ? 'var(--completed-bg)' : 'var(--card-bg)',
      boxShadow: 'var(--shadow)',
      display: 'flex',
      justifyContent: 'space-between',
      alignItems: 'center',
      transition: 'all 0.3s ease'
    }}>
      <div>
        <h3 style={{ 
          textDecoration: task.completed ? 'line-through' : 'none',
          color: task.completed ? 'var(--completed-text)' : 'var(--text-color)',
          margin: '0 0 8px 0'
        }}>
          {task.header}
        </h3>
        <p style={{ 
          color: 'var(--text-color)',
          margin: '0 0 8px 0'
        }}>
          {task.description}
        </p>
        <div style={{ fontSize: '0.8rem', color: 'var(--text-secondary)' }}>
          <span>Difficulty: {task.difficulty} | </span>
          {task.dueDate && <span>Due: {task.dueDate}</span>}
        </div>
      </div>
      <div>
        <button 
          onClick={handleComplete}
          style={{
            marginRight: '8px',
            backgroundColor: task.completed ? 'var(--text-secondary)' : 'var(--success-color)',
            color: 'white',
            border: 'none',
            padding: '8px 16px',
            borderRadius: '4px',
            cursor: 'pointer',
            transition: 'background-color 0.2s ease'
          }}
        >
          {task.completed ? 'Undo' : 'Complete'}
        </button>
        <button 
          onClick={handleDelete}
          style={{
            backgroundColor: 'var(--danger-color)',
            color: 'white',
            border: 'none',
            padding: '8px 16px',
            borderRadius: '4px',
            cursor: 'pointer',
            transition: 'background-color 0.2s ease'
          }}
        >
          Delete
        </button>
      </div>
    </div>
  );
};

export default TaskItem;

