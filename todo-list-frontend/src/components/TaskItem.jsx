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
      borderRadius: '4px',
      border: '1px solid #ddd',
      backgroundColor: task.completed ? '#f8f9fa' : 'white',
      display: 'flex',
      justifyContent: 'space-between',
      alignItems: 'center'
    }}>
      <div>
        <h3 style={{ 
          textDecoration: task.completed ? 'line-through' : 'none',
          color: task.completed ? '#6c757d' : 'black'
        }}>
          {task.header}
        </h3>
        <p>{task.description}</p>
        <div style={{ fontSize: '0.8rem', color: '#6c757d' }}>
          <span>Difficulty: {task.difficulty} | </span>
          {task.dueDate && <span>Due: {task.dueDate}</span>}
        </div>
      </div>
      <div>
        <button 
          onClick={handleComplete}
          style={{
            marginRight: '5px',
            backgroundColor: task.completed ? '#6c757d' : '#28a745',
            color: 'white',
            border: 'none',
            padding: '6px 12px',
            borderRadius: '4px',
            cursor: 'pointer'
          }}
        >
          {task.completed ? 'Undo' : 'Complete'}
        </button>
        <button 
          onClick={handleDelete}
          style={{
            backgroundColor: '#dc3545',
            color: 'white',
            border: 'none',
            padding: '6px 12px',
            borderRadius: '4px',
            cursor: 'pointer'
          }}
        >
          Delete
        </button>
      </div>
    </div>
  );
};

export default TaskItem;

