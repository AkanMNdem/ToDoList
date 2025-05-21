import React, { useState, useEffect } from 'react';
import { getTasks, getPrioritizedTasks, getCompletedTasks } from '../services/taskService';
import TaskItem from './TaskItem';

const TaskList = ({ refreshTrigger }) => {
  const [tasks, setTasks] = useState([]);
  const [viewMode, setViewMode] = useState('all'); // 'all', 'prioritized', 'completed'
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    fetchTasks();
  }, [viewMode, refreshTrigger]);

  const fetchTasks = async () => {
    setLoading(true);
    try {
      let data;
      switch (viewMode) {
        case 'prioritized':
          data = await getPrioritizedTasks();
          break;
        case 'completed':
          data = await getCompletedTasks();
          break;
        default:
          data = await getTasks();
          break;
      }
      setTasks(data.tasks || []);
    } catch (error) {
      console.error('Error fetching tasks:', error);
    } finally {
      setLoading(false);
    }
  };

  return (
    <div>
      <div style={{ display: 'flex', marginBottom: '20px' }}>
        <button 
          onClick={() => setViewMode('all')}
          style={{
            backgroundColor: viewMode === 'all' ? '#007bff' : '#f8f9fa',
            color: viewMode === 'all' ? 'white' : 'black',
            border: '1px solid #ddd',
            padding: '8px 16px',
            borderRadius: '4px 0 0 4px',
            cursor: 'pointer'
          }}
        >
          All Tasks
        </button>
        <button 
          onClick={() => setViewMode('prioritized')}
          style={{
            backgroundColor: viewMode === 'prioritized' ? '#007bff' : '#f8f9fa',
            color: viewMode === 'prioritized' ? 'white' : 'black',
            border: '1px solid #ddd',
            borderLeft: 'none',
            padding: '8px 16px',
            cursor: 'pointer'
          }}
        >
          Prioritized
        </button>
        <button 
          onClick={() => setViewMode('completed')}
          style={{
            backgroundColor: viewMode === 'completed' ? '#007bff' : '#f8f9fa',
            color: viewMode === 'completed' ? 'white' : 'black',
            border: '1px solid #ddd',
            borderLeft: 'none',
            padding: '8px 16px',
            borderRadius: '0 4px 4px 0',
            cursor: 'pointer'
          }}
        >
          Completed
        </button>
      </div>

      {loading ? (
        <p>Loading tasks...</p>
      ) : tasks.length === 0 ? (
        <p>No tasks found.</p>
      ) : (
        <div>
          {tasks.map(task => (
            <TaskItem 
              key={task.id} 
              task={task} 
              onTaskUpdated={fetchTasks} 
            />
          ))}
        </div>
      )}
    </div>
  );
};

export default TaskList;
