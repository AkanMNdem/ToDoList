import React from 'react';
import { useTheme, THEMES } from '../contexts/ThemeContext';

const ThemeToggle = () => {
  const { theme, setTheme, THEMES } = useTheme();

  const handleThemeChange = (e) => {
    setTheme(e.target.value);
  };

  return (
    <div className="theme-toggle" style={{
      position: 'absolute',
      top: '20px',
      right: '20px',
      display: 'flex',
      alignItems: 'center',
      gap: '10px',
      background: 'var(--card-bg)',
      padding: '8px 12px',
      borderRadius: '8px',
      boxShadow: '0 2px 5px rgba(0,0,0,0.1)'
    }}>
      <label htmlFor="theme-select" style={{ fontSize: '14px', color: 'var(--text-color)' }}>
        Theme:
      </label>
      <select 
        id="theme-select"
        value={theme}
        onChange={handleThemeChange}
        style={{
          background: 'var(--bg-secondary)',
          color: 'var(--text-color)',
          border: '1px solid var(--border-color)',
          padding: '5px 8px',
          borderRadius: '4px',
          cursor: 'pointer',
          outline: 'none'
        }}
      >
        <option value={THEMES.LIGHT}>Light</option>
        <option value={THEMES.DARK}>Dark</option>
        <option value={THEMES.SYSTEM}>System</option>
      </select>
    </div>
  );
};

export default ThemeToggle; 