import React, { useState } from 'react';
import TaskForm from './components/TaskForm';
import TaskList from './components/TaskList';
import ThemeToggle from './components/ThemeToggle';
import { ThemeProvider } from './contexts/ThemeContext';
import './App.css';

function App() {
  const [refreshKey, setRefreshKey] = useState(0);

  const handleTaskUpdated = () => {
    setRefreshKey(prevKey => prevKey + 1);
  };

  return (
    <ThemeProvider>
      <div className="App" style={{ 
        maxWidth: '800px', 
        margin: '0 auto', 
        padding: '20px',
        position: 'relative',
        backgroundColor: 'var(--bg-secondary)',
        borderRadius: '8px',
        boxShadow: 'var(--shadow)',
        minHeight: '90vh'
      }}>
        <ThemeToggle />
        
        <header style={{ 
          marginBottom: '30px', 
          textAlign: 'center',
          padding: '20px 0'
        }}>
          <h1 style={{ color: 'var(--text-color)' }}>Todo List App</h1>
          <p style={{ color: 'var(--text-secondary)' }}>Manage your tasks efficiently</p>
        </header>

        <TaskForm onTaskAdded={handleTaskUpdated} />
        <TaskList refreshTrigger={refreshKey} />
      </div>
    </ThemeProvider>
  );
}

export default App;
