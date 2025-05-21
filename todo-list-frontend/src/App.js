import React, { useState } from 'react';
import TaskForm from './components/TaskForm';
import TaskList from './components/TaskList';
import './App.css';

function App() {
  const [refreshKey, setRefreshKey] = useState(0);

  const handleTaskUpdated = () => {
    setRefreshKey(prevKey => prevKey + 1);
  };

  return (
    <div className="App" style={{ maxWidth: '800px', margin: '0 auto', padding: '20px' }}>
      <header style={{ marginBottom: '30px', textAlign: 'center' }}>
        <h1>Todo List App</h1>
        <p>Manage your tasks efficiently</p>
      </header>

      <TaskForm onTaskAdded={handleTaskUpdated} />
      <TaskList refreshTrigger={refreshKey} />
    </div>
  );
}

export default App;
