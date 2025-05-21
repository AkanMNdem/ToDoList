import React, { useState, useEffect } from 'react';
import { getTasks, getPrioritizedTasks, getCompletedTasks } from '../services/taskService';
import TaskItem from './TaskItem';

const TaskList = ({ refreshTrigger }) => {
  const [tasks, setTasks] = useState([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState('');
  const [view, setView] = useState('all'); // 'all', 'prioritized', 'completed'

  useEffect(() => {
    fetchTasks();
  }, [refreshTrigger, view]);

  const fetchTasks = async () => {
    setLoading(true);
    setError('');
    try {
      let data;
      console.log(`Fetching tasks for view: ${view}`);
      
      switch (view) {
        case 'prioritized':
          data = await getPrioritizedTasks();
          break;
        case 'completed':
          data = await getCompletedTasks();
          break;
        default:
          data = await getTasks();
      }
      
      console.log('Tasks received:', data);
      
      if (!data || !data.tasks) {
        console.error('Invalid data format:', data);
        setError('Received invalid data format from server');
        setTasks([]);
      } else {
        setTasks(data.tasks || []);
        setError('');
      }
    } catch (error) {
      console.error('Error fetching tasks:', error);
      setError(`Failed to load tasks: ${error.message}`);
      setTasks([]);
    } finally {
      setLoading(false);
    }
  };

  const handleViewChange = (newView) => {
    setView(newView);
  };

  return (
    <div className="task-list" style={{ 
      backgroundColor: 'var(--card-bg)',
      borderRadius: '8px',
      padding: '20px',
      boxShadow: 'var(--shadow)',
      border: '1px solid var(--border-color)'
    }}>
      <div style={{ 
        display: 'flex', 
        justifyContent: 'space-between', 
        alignItems: 'center',
        marginBottom: '20px'
      }}>
        <h2 style={{ 
          margin: '0',
          color: 'var(--text-color)'
        }}>
          {view === 'prioritized' ? 'Prioritized Tasks' : 
           view === 'completed' ? 'Completed Tasks' : 'All Tasks'}
        </h2>
        
        <div className="view-switcher" style={{ 
          display: 'flex',
          gap: '10px'
        }}>
          <button 
            onClick={() => handleViewChange('all')} 
            style={{
              backgroundColor: view === 'all' ? 'var(--primary-color)' : 'var(--bg-secondary)',
              color: view === 'all' ? 'white' : 'var(--text-color)',
              border: `1px solid ${view === 'all' ? 'var(--primary-color)' : 'var(--border-color)'}`,
              padding: '8px 12px',
              borderRadius: '4px',
              cursor: 'pointer'
            }}
          >
            All
          </button>
          <button 
            onClick={() => handleViewChange('prioritized')} 
            style={{
              backgroundColor: view === 'prioritized' ? 'var(--primary-color)' : 'var(--bg-secondary)',
              color: view === 'prioritized' ? 'white' : 'var(--text-color)',
              border: `1px solid ${view === 'prioritized' ? 'var(--primary-color)' : 'var(--border-color)'}`,
              padding: '8px 12px',
              borderRadius: '4px',
              cursor: 'pointer'
            }}
          >
            Prioritized
          </button>
          <button 
            onClick={() => handleViewChange('completed')} 
            style={{
              backgroundColor: view === 'completed' ? 'var(--primary-color)' : 'var(--bg-secondary)',
              color: view === 'completed' ? 'white' : 'var(--text-color)',
              border: `1px solid ${view === 'completed' ? 'var(--primary-color)' : 'var(--border-color)'}`,
              padding: '8px 12px',
              borderRadius: '4px',
              cursor: 'pointer'
            }}
          >
            Completed
          </button>
        </div>
      </div>

      {loading ? (
        <div style={{ 
          textAlign: 'center', 
          padding: '20px',
          color: 'var(--text-secondary)'
        }}>
          Loading tasks...
        </div>
      ) : error ? (
        <div style={{ 
          padding: '15px', 
          backgroundColor: 'var(--danger-color)', 
          color: 'white', 
          borderRadius: '4px',
          marginBottom: '15px'
        }}>
          {error}
        </div>
      ) : tasks.length === 0 ? (
        <div style={{ 
          textAlign: 'center', 
          padding: '30px',
          color: 'var(--text-secondary)',
          backgroundColor: 'var(--bg-primary)',
          borderRadius: '8px',
          border: '1px dashed var(--border-color)'
        }}>
          No tasks found. Add some tasks to get started!
        </div>
      ) : (
        tasks.map(task => (
          <TaskItem key={task.id} task={task} onTaskUpdated={fetchTasks} />
        ))
      )}
    </div>
  );
};

export default TaskList;
