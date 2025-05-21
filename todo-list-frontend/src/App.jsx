import React, { useState, useEffect } from 'react';
import TaskForm from './components/TaskForm';
import TaskList from './components/TaskList';
import ThemeToggle from './components/ThemeToggle';
import { ThemeProvider } from './contexts/ThemeContext';
import axios from 'axios';

function App() {
  const [refreshTrigger, setRefreshTrigger] = useState(0);
  const [apiStatus, setApiStatus] = useState({ checking: true, ok: false, message: 'Checking API connection...' });

  // Function to check API connectivity
  const checkApiConnection = async () => {
    setApiStatus({ checking: true, ok: false, message: 'Checking API connection...' });
    try {
      // Use relative URL for proxy to work
      const response = await axios.get('/health');
      if (response.status === 200 && response.data.status === 'ok') {
        setApiStatus({ checking: false, ok: true, message: 'API connected' });
      } else {
        setApiStatus({ checking: false, ok: false, message: 'API returned unexpected response' });
      }
    } catch (error) {
      console.error('API Connection Error:', error);
      setApiStatus({ 
        checking: false, 
        ok: false, 
        message: `API connection failed: ${error.message}`
      });
    }
  };

  // Check API connection on component mount
  useEffect(() => {
    checkApiConnection();
  }, []);

  const handleTaskAdded = () => {
    setRefreshTrigger(prev => prev + 1);
  };

  return (
    <ThemeProvider>
      <div style={{ 
        maxWidth: '800px', 
        margin: '0 auto', 
        padding: '20px',
        backgroundColor: 'var(--bg-primary)',
        color: 'var(--text-color)',
        minHeight: '100vh',
        transition: 'all 0.3s ease'
      }}>
        <header style={{ 
          marginBottom: '30px',
          textAlign: 'center'
        }}>
          <h1 style={{ 
            color: 'var(--primary-color)',
            marginBottom: '5px'
          }}>Todo List App</h1>
          <p style={{ 
            color: 'var(--text-secondary)',
            fontSize: '1.1rem'
          }}>Manage your tasks efficiently</p>
          <ThemeToggle />
          
          {/* API Status Indicator */}
          <div style={{
            marginTop: '10px',
            padding: '8px',
            backgroundColor: apiStatus.checking ? 'var(--warning-color)' : 
                            apiStatus.ok ? 'var(--success-color)' : 'var(--danger-color)',
            color: 'white',
            borderRadius: '4px',
            fontSize: '0.9rem',
            display: 'inline-block'
          }}>
            {apiStatus.message}
            {!apiStatus.ok && !apiStatus.checking && (
              <button 
                onClick={checkApiConnection}
                style={{
                  marginLeft: '10px',
                  backgroundColor: 'white',
                  color: 'var(--danger-color)',
                  border: 'none',
                  borderRadius: '4px',
                  padding: '2px 8px',
                  cursor: 'pointer',
                  fontSize: '0.8rem'
                }}
              >
                Retry
              </button>
            )}
          </div>
        </header>
        
        <main>
          <TaskForm onTaskAdded={handleTaskAdded} />
          <TaskList refreshTrigger={refreshTrigger} />
        </main>
      </div>
    </ThemeProvider>
  );
}

export default App; 